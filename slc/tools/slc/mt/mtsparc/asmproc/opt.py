import re
import common
from ..regdefs import regmagic

_default_presets = set(("$%d" % x for x in xrange(31)))
def grouper(items):
    """
    Group function code together.
    Produce a generator of (type, code, comment).
    """

    presets = _default_presets.copy()
    presets.add("$l0")

    state = 0
    queue = []
    for (type, content, comment) in items:
        if state == 0:
            if content.startswith('.type') and content.endswith("#function"):
                name = content.split(None)[1].split(',')[0]
                state = 1
                queue.append((type, content, comment))
                continue
        else:
            queue.append((type, content, comment))
            if (not content.startswith('.size')) or content.split(None)[1].split(',')[0] != name:
                continue
            yield ('fun',{'name':name,'body':queue,'presets':presets},'')
            queue = []
            state = 0
            continue
        for q in queue:
            yield q
        queue = []
        state = 0
        yield (type, content, comment)

_re_vname = re.compile(r'%(r|t[dsgl])(f?\d+)')
_re_canon = re.compile(r'\$([dsgl]?f?\d+)')
def vnametocanon(fundata, items):
        def repl(match):
                t = match.group(1)
                n = match.group(2)
                t = t[1:]
                return '$%s%s' % (t, n)
                
        for (type,content,comment) in items:
                content = _re_vname.sub(repl, content)
                comment = _re_vname.sub(repl, comment)
                yield (type, content, comment)
        

def canontovname(fundata, items):
        def repl(match):
                t = match.group(1)
                if t[0] in "dsgl":
                        p = 't'
                else:
                        p = 'r'
                return '%%%s%s' % (p, t)
        for (type,content,comment) in items:
                content = _re_canon.sub(repl, content)
                comment = _re_canon.sub(repl, comment)
                yield (type, content, comment)

_re_regnum = re.compile('([0-9]+)')
def nextreg(reg):
    return _re_regnum.sub(lambda n:str(int(n.group(1))+1), reg)

# seereg looks at registers that are written by threads (locals, dependents)
_re_seereg = re.compile(r'\$[ld]?f?\d+')
def seereg(reg, str):
    for m in _re_seereg.findall(str):
        if m == reg:
            return True
        return False

_re_creargs = re.compile(r'.*MT: CREATE.*DRAIN\(([^)]*)\)')
def makedataflow(addswch, drainargs):
    class DetectDataflow(object):
        def __init__(self):
            if addswch:
                self.__name__ = "addswchll"
            else:
                self.__name__ = "dataflow"

        def __call__(self, fundata, items):
            """
            Add "swch" annotations after instructions that may consume
            the result of a long-latency operation.

            Additionally, as an optimization, insert a "swch" annotation before
            as well, to give some extra delay to the long-latency operations
            by forcing a cycle through the thread active queue.
            """
            allregs = regmagic.allregs

            swchbeforell = None
            if addswch:
                swchbeforell = self.extra_options.get('swch-before-ll-use',True)

            locals_offset = regmagic.rd.mt_locals_offset
            swchenabled = True

            # On UT-LEON3, we can't switch on a branch/delayed instruction that
            # reads from a waiting register, in which case we must ensure that
            # every register for which there is an uncertainy is full before the
            # instruction is reached. This flag can be enabled from the command-line
            # with -fasmopt-flush-before-delayed.
            flushbeforedelayed = self.extra_options.get('flush-before-delayed',False)

            # initially exclude index
            maybell = allregs.copy()
            for (type, content, comment) in items:
                #print (type,content[:3],comment)
                if type == 'label':
                    #print "FOO"
                    yield (type, content, comment)
                    maybell = allregs.copy()

                elif type == 'empty':
                    yield (type, content, comment)
                    if comment.startswith('MTREG_SET:'):
                        hint = set(comment.split(':',1)[1].strip().split(','))
                        maybell = maybell - hint
                    elif comment.startswith('MTREG_SPEC:'):
                        hint = set(comment.split(':',1)[1].strip().split(','))
                        maybell = maybell - hint
                        allregs = allregs - hint

                elif type == 'directive' and content in ['.swch', '.noswch']:
                    swchenabled = (content == '.swch')

                elif type == 'other' and content.metadata is not None:
                    #print "BAR %s :: %s ::" % (content, comment)
                    reads = []
                    shortwrites = []
                    longwrites = []

                    md = content.metadata
                    ops = content.operands
                    ll = md.long_latency

                    for i in md.inputs:
                        op = ops[i]
                        reads.append(op)
                        if i in md.double_regs:
                            reads.append(nextreg(op))
                            #print "hello double",i,reads
                    for i in md.extra_inputs:
                        if i == 'y': continue
                        reads.append('$%d' % i)
                        reads.append('$l%d' % (i-locals_offset))

                    if ll:
                        outputs = longwrites
                    else:
                        outputs = shortwrites

                    for i in md.outputs:
                        op = ops[i]
                        outputs.append(op)
                        if i in md.double_regs:
                            outputs.append(nextreg(op))

                    for i in md.extra_outputs:
                        if i == 'y': continue
                        outputs.append('$%d' % i)
                        outputs.append('$l%d' % (i-locals_offset))

                    if drainargs and content.startswith('create'):
                        m = _re_creargs.match(comment)
                        if m is not None:
                            argregs = m.group(1).split(',')
                            removed = set()
                            for rx in maybell:
                                for r in argregs:
                                    if seereg(rx, r):
                                        yield('other', 'mov %s,%s' %(r,r), 'MT: drain create arg')
                                        yield('other', 'swch', '')
                                        #print "XXX", argregs
                                        removed.add(r)
                            maybell = maybell - removed


                    test = 0

                    # "q" is the set of touched registers -- those that
                    # hold a ready value after the current instruction.
                    q = set()

                    # if one of the "maybe regs" is read from,
                    # assume we are reading result from long latency.
                    #print "maybell = %r" % (maybell,)
                    #print "reads = %r" % (reads,)
                    for rx in maybell:
                        for r in reads:
                            if seereg(rx,r):
                                test = 1
                                q.add(rx)

                    if test == 1 and md.delayed and flushbeforedelayed:
                        # we can't suspend a delayed instruction,
                        # so we need to force all inputs beforehand
                        for r in q:
                            yield ('other', 'mov %s,%s' %(r,r), 'MT: force read before delayed')
                            yield ('other', 'swch', '')
                        test = 0

                    if test == 1 and addswch and swchbeforell and swchenabled:
                        yield ('other','swch','MT: before-ll: %s' % ','.join(q))
                    yield (type, content, comment)
                    if test == 1 and addswch and swchenabled:
                        yield ('other','swch','MT: after-ll: %s' % ','.join(q))

                    if content.metadata.is_branch:
                        maybell = allregs.copy()
                    else:
                        # all the registers that were "short written"
                        # will not cause long latency during next use
                        for rx in maybell:
                            for r in shortwrites:
                                if seereg(rx, r):
                                    q.add(rx)
                        # remove touched registers from the long latency registers
                        maybell = maybell - q
                        # add generated long latency
                        for rx in allregs:
                            for r in longwrites:
                                if seereg(rx, r):
                                    maybell.add(rx)
                else:
                    yield (type, content, comment)

    return DetectDataflow()

def protectend(fundata, items):
    """
    Insert a "nop" instruction just before "end" 
    when "end" immediately succeeds a label or a delay slot
    """
    atlabel = True
    for (type, content, comment) in items:
        if type == 'label' or (type == 'other' and 'DELAYSLOT' in comment):
            atlabel = True
        elif type == 'other':
            if content == 'end' and atlabel:
                yield ('other', 'nop', 'MT: end after label/delayslot')
            atlabel = False
        yield (type, content, comment)

from ...common.asmproc.remdefaults import *
from ...common.asmproc.labels import *
from ...common.asmproc.swch import *
from ...common.asmproc.end import *
from decode import decode
from delay import *

_filter_begin = [common.reader, 
                 common.lexer, 
                 common.splitsemi, 
                 common.parser,
                 grouper]
_filter_stages = [
        vnametocanon,
        remdefaults,

        findlabels,
        killunusedlabels,

        decode,
        makedataflow(addswch = True, drainargs = False),

        rmdupswch,
        rmswchbegin,

        prunenopend,

        markdelay,
        protectend,

        canontovname,

        rmswchdir,
        ]
_filter_end = [common.flattener,
               common.printer]
                 


from ...common.asmproc import enhance

def filter(*args):
	return enhance.filter(_filter_begin, _filter_stages, _filter_end, *args)

