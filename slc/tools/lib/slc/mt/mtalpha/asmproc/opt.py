import re
import base
import os
from ....msg import die, log
from ..regdefs import regmagic
from ....front import opts, dump
import  common

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
            if content.startswith('.ent'):
                name = content.split(' ',1)[1]
                state = 1
                queue.append((type, content, comment))
                continue
        else:
            queue.append((type, content, comment))
            if not content.startswith('.end'):
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



_re_movr = re.compile(r'mov\s+(\$[gsld]?f\d+.*)')
def adjustmov(fundata, items):
    """
    Code generation may have used "mov" instructions to copy
    floating-point registers. These need to be replaced by "fmov".

    FIXME: this is maybe not needed any more?
    """
    for (type, content, comment) in items:
        if type == 'other':
            m = _re_movr.match(content)
            if m is not None:
                yield (type, "fmov " + m.group(1), comment)
                continue
        yield (type, content, comment)


from ...common.asmproc.remdefaults import *
from ...common.asmproc.labels import *


_re_seereg = re.compile(r'\$[ld]?f?\d+')
def seereg(reg, str):
    for m in _re_seereg.findall(str):
        if m == reg:
            return True
        return False

_re3way = re.compile(r'\w+\s([^,]+),([^,]+),([^,]+)$')
_re1way = re.compile(r'\w+\s([^,]+)$')
_re2way = re.compile(r'(?:mov|sqrt\S*|cvt\S*|itof\S*|ftoi\S*|sync|allocate)\s([^,]+),([^,]+)$')
_rexway = re.compile(r'\w+\s([^,]+),(.*)')
_reld = re.compile(r'(ld[lqts]|ld[bw]u)')
_re3wayll = re.compile(r'((add|sub|mul|div)[fgts]|f?gets)')
_w1way = re.compile(r'(rpcc|ldfp|ldpc|f?clr)')


def addswchll(fundata, items):
    """
    Add "swch" annotations after instructions that may consume
    the result of a long-latency operation.

    Additionally, as an optimization, insert a "swch" annotation before
    as well, to give some extra delay to the long-latency operations
    by forcing a cycle through the thread active queue.
    """
    re3way = _re3way
    re1way = _re1way
    re2way = _re2way
    rexway = _rexway
    reld = _reld
    re3wayll = _re3wayll
    w1way = _w1way
    jre = base._re_jsr

    allregs = regmagic.allregs
        
    swchbeforell = not 'no-swch-before-ll-use' in addswchll.extra_options

    # initially exclude index
    maybell = allregs.copy()
    for (type, content, comment) in items:
        #print (type,content[:3],comment)
        if type == 'label' or (type == 'other' and jre.match(content)):
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

        elif type == 'other':
            #print "BAR %s :: %s ::" % (content, comment)
            reads = []
            shortwrites = []
            longwrites = []
            done = 0

            if not done:
                # check 3-way insns
                m = re3way.match(content)
                if m is not None:
                    r1, r2, r3 = m.groups()
                    reads.append(r1)
                    reads.append(r2)
                    if re3wayll.match(content) is None:
                        shortwrites.append(r3)
                    else:
                        longwrites.append(r3)
                    done = 1

            if not done:
                # check 1-way insn, 1st is target (e.g. break)
                m = re1way.match(content)
                if m is not None:
                    r1 = m.group(1)
                    if w1way.match(content) is not None:
                        shortwrites.append(r1)
                    else:
                        reads.append(r1)
                    done = 1


            if not done:
                # check 2-way insn, second is target (e.g. mov)
                m = re2way.match(content)
                if m is not None:
                    r1, r2 = m.groups()
                    reads.append(r1)
                    if content.startswith('sqrt') or content.startswith('sync') or content.startswith('allocate'):
                        longwrites.append(r2)
                    else:
                        shortwrites.append(r2)
                    done = 1

            if not done:
                # check other insns, assume 1st is target
                m = rexway.match(content)
                if m is not None:
                    r1, r2 = m.groups()
                    reads.append(r2)

                    # Special case for loads from memory:
                    m2 = reld.match(content)
                    if m2 is not None:
                        longwrites.append(r1)
                    elif content.startswith('lda') or content.startswith('ldgp'):
                        #print "FOO"
                        shortwrites.append(r1)
                    elif content.startswith('cred') \
                             or content.startswith('crei'):
                        longwrites.append(r1)
                        reads.append(r1)
                    else:
                        # insn reads 1st operand, then writes to it
                        reads.append(r1)
                        shortwrites.append(r1)
                    done = 1

            if not done:
                # write barrier and other insns that have no args
                pass
            
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
            if test == 1 and swchbeforell:
                yield ('other','swch','MT: before-ll: %s' % ','.join(q))
            yield (type, content, comment)
            if test == 1:
                yield ('other','swch','MT: after-ll: %s' % ','.join(q))

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


_re_br = re.compile(r'(f?(beq|bne|bge|ble|bgt|blt|blbc)|br|jmp)\s')
_re_bsr = re.compile(r'(bsr|jsr|ret|jsr_coroutine)\s')
def addswchbr(fundata, items):
    """
    Insert "swch" after branch instructions.
    """
    for (type, content, comment) in items:
        yield (type, content, comment)
        if _re_br.match(content) is not None:
            yield ('other','swch','MT: branch')
        elif _re_bsr.match(content) is not None:
            yield ('other','swch','MT: sbranch')


from ...common.asmproc.swch import *

from ...common.asmproc.end import *


def protectend(fundata, items):
    """
    Insert a "nop" instruction just before "end" 
    when "end" immediately succeeds a label or a branch to subroutine.
    """
    atlabel = True
    for (type, content, comment) in items:
        if type == 'label' or (type == 'other' and 
                               (_re_bsr.match(content) is not None or \
                                    (content == 'swch' and comment == 'MT: sbranch'))):
            atlabel = True
        elif type == 'other':
            if content == 'end' and atlabel:
                yield ('other', 'nop', 'MT: end after label/bsr')
            atlabel = False
        yield (type, content, comment)

_re_deps = re.compile(r'.*\$df?\d+')
def addswchdep(fundata, items):
    """
    Insert "swch" after instructions that read a dependent register.

    FIXME: remove? This is not used any more.
    """
    for (type, content, comment) in items:
        yield (type, content, comment)
        if _re_deps.match(content) is not None:
            yield ('other','swch','MT: previous insn reads dependent')

_re_se = re.compile(r'setempty\s(\$lf.*)$')
def fixemptyf(fundata, items):
    """
    Replace "setempty" instructions that operate
    on FP registers by "setemptyf".

    FIXME: remove? This is not used any more.
    """
    for (type, content, comment) in items:
        if type == 'other':
            m = _re_se.match(content)
            if m is not None:
                content = 'setemptyf %s' % m.group(1)
        yield (type, content, comment)


_filter_begin = [common.reader, 
                 common.lexer, 
                 common.splitsemi, 
                 common.parser, 
                 grouper]
_filter_stages = [adjustmov, 
                  fixemptyf,
                  remdefaults, 
                  
                  findlabels, 
                  killunusedlabels,
                  
                  addswchll, 
                  addswchbr, 
                  rmdupswch, 
                  rmswchbegin,
                  
                  prunenopend, 
                  protectend]

_filter_end = [common.flattener,
               common.forcezero,
               common.printer]

from ...common.asmproc import enhance

def filter(*args):
    return enhance.filter(_filter_begin, _filter_stages, _filter_end, *args)

