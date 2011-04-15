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

_re_seereg = re.compile(r'\$[gld]?f?\d+')
def seereg(reg, str):
    for m in _re_seereg.findall(str):
        if m == reg:
            return True
        return False

def addswchll(fundata, items):
    """
    Add "swch" annotations after instructions that may consume
    the result of a long-latency operation.

    Additionally, as an optimization, insert a "swch" annotation before
    as well, to give some extra delay to the long-latency operations
    by forcing a cycle through the thread active queue.
    """
    allregs = regmagic.allregs
        
    swchbeforell = addswchll.extra_options.get('swch-before-ll-use',True)

    locals_offset = regmagic.rd.mt_locals_offset

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
                
            
            test = 0
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

            if test == 1 and md.delayed:
                # we don't support swch after a delayed instruction,
                # so we need to force all inputs beforehand
                for r in q:
                    yield ('other', 'mov %s,%s' %(r,r), 'MT: force read before delayed')
                    yield ('other', 'swch', '')
                test = 0
            if test == 1 and swchbeforell:
                yield ('other','swch','MT: before-ll: %s' % ','.join(q))
            yield (type, content, comment)
            if test == 1:
                yield ('other','swch','MT: after-ll: %s' % ','.join(q))

            if content.metadata.is_branch:
                maybell = allregs.copy()
            else:
                # all the registers that are "short written"
                # will not cause long latency during next use
                for rx in maybell:
                    for r in shortwrites:
                        if seereg(rx, r):
                            q.add(rx)
                # remove touched registers from the long latency registers
                for r in q:
                    maybell = set((x for x in maybell if x != r))
                # add generated long latency
                for rx in allregs:
                    for r in longwrites:
                        if seereg(rx, r):
                            maybell.add(rx)
        else:
            yield (type, content, comment)



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
        addswchll,

        rmdupswch,
        rmswchbegin,

        prunenopend,

        markdelay,
        protectend,

        canontovname,
        ]
_filter_end = [common.flattener,
               common.printer]
                 


from ...common.asmproc import enhance

def filter(*args):
	return enhance.filter(_filter_begin, _filter_stages, _filter_end, *args)

