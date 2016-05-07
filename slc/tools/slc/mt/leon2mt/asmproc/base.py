from ..regdefs import regmagic
from ...mtsparc.asmproc.decode import decode
from ...mtsparc.asmproc.common import *
from ...common.asmproc.regextract import *
from ...common.asmproc.renameregs import *
from ...common.asmproc.labels import *
from ...common.asmproc.markused import *
from ...common.asmproc.compress import *
from ...mtsparc.asmproc.delay import *
from ...mtsparc.asmproc.opt import makedataflow
from ...mtalpha.asmproc.base import killspadjusts,avoidframespills
from ...mtsparc.asmproc.base import detectregs, cdetectregs, renamelocals, replaceret, xsplit, \
    xjoin1, \
    grouper, \
    cmarkused, \
    protectallcallregs, \
    protectend, \
    fillannulnop, \
    markaligned, \
    crenameregs, \
    renameregs, \
    zerog0

_re_rasave = re.compile(r'mov\s+(\$l\d+)')
def killrasave(fundata, items):
    ra1reg = regmagic.alias_to_vname('ra')
    ra2reg = regmagic.alias_to_vname('ra_leaf')
    p = []
    for (type, content, comment) in fundata['prologue']:
        m = _re_rasave.match(content)
        if type == 'other' and m is not None and m.group(1) in [ra1reg, ra2reg]:
            p.append(('empty','','MT: killed ra save: ' + content))
            continue
        p.append((type, content, comment))
    fundata['prologue'] = p
    return items

def killnoregs(fundata, items):
    for (type, content, comment) in items:
        if type == 'directive' and content == '.noregisters':
            continue
        yield (type, content, comment)

_re_munchret = re.compile(r'(ld.?|add|mov|nop|fnop|unop)\s+')
def munchret(fundata, items):
    """
    Remove possible frame adjustments before 'end'.
    """
    rekill = _re_munchret

    queue = []
    for (type, content, comment) in items:
        if type == 'other' and content == 'end':
            hello = 1
            qkill = []
            qpreserve = []
            while hello:
                if len(queue) == 0:
                    hello = 0
                else:
                    (t,c,cm) = queue[-1]
                    if t in ['empty','directive']:
                        qpreserve.insert(0, (t,c,cm))
                        queue.pop()
                    elif t == 'other' and \
                             rekill.match(c) and \
                             '$s' not in c and \
                             '$d' not in c and \
                             'SYNC' not in cm:
                        qkill.insert(0,(t,c,cm))
                        queue.pop()
                    else:
                        hello = 0
            for (type, content, comment) in qkill:
                queue.append(('empty','','MT: killed cleanup: %s' % content))
            for r in qpreserve:
                queue.append(r)
            queue.append(('other','end','MT: end thread'))
        else:
            queue.append((type, content, comment))

    return queue

def initspfp(fundata, items):
    """
    If SP/FP are needed, initialize them in the prologue.
    """
    spreg = regmagic.alias_to_vname("tlsp")
    fpreg = regmagic.alias_to_vname("fp")
    newp = []
    if fundata['use_sp'] or fundata['use_fp']:
        if fundata['use_sp']:
                newp += [
                    ('other', 't_get_tid %s' % spreg, 'MT: init SP'),
                    ('other', 'sethi %hi(__first_tls_top), $l1', 'MT: init SP'),
                    ('other', 'sll %s,10,%s' % (spreg,spreg), 'MT: init SP'),
                    ('other', 'add %s,$l1,%s' % (spreg,spreg), 'MT: init SP'),
                    ]
        if fundata['use_fp'] and fundata['killedsave']:
                if fundata['use_sp']:
                    newp.append(('other', 'mov %s, %s' % (spreg, fpreg), 'MT: init FP'))
                else:
                    newp += [
                    ('other', 't_get_tid %s' % fpreg, 'MT: init FP'),
                    ('other', 'sethi %hi(__first_tls_top), $l1', 'MT: init FP'),
                    ('other', 'sll %s,10,%s' % (fpreg,fpreg), 'MT: init FP'),
                    ('other', 'add %s,$l1,%s' % (fpreg,fpreg), 'MT: init FP'),
                    ] 
    fundata['prologue'] = newp + fundata['prologue']
    return items


def ctlbits(items):
    for (type, content, comment) in items:
        if type in ["fun", "cfun"]:
            yield ('directive', '.align 128', '')
            yield ('directive', '.ctlbits 128 0', '')
        yield (type, content, comment)
        

# FIXME: make this configurable?
lrbsz = 32

def xjoin2(fundata, items):
    """
    Add function header and footer.
    """

    name = fundata['name']
    regs = fundata['regs']

    if fundata['global']:
        yield ('directive', '.global\t%s' % name, '')
    yield ('directive', '.type\t%s, #function' % name, '')
    lpt = regs[2]
    if lpt == 0:
        lpt = 1
    tpb = lrbsz // lpt
    yield ('directive', '.long %d' % (tpb * lrbsz + lpt), 'MT: LAYOUT %d %d %d' % (lpt, tpb, regs[0]))
    yield ('label', '%s:' % name, '')

    p = fundata['presets']
    
    for (type, content, comment) in items:
        if type == 'empty' and comment == 'MTREG_SET':
            # FIXME: ADD THIS WHEN SPECIALS ARE IMPLEMENTED
            # if fundata['use_sp']: yield ('empty','','MTREG_SPEC: $l%d' % fundata['specials']['tlsp'])
            yield ('empty', '', 'MTREG_SET: %s' % ','.join(p))
        else:
            yield (type, content, comment)

    yield ('directive', '.size\t%s, .-%s' % (name, name), '')

def replacebreakend(fundata, items):
    """
    Replace 'end' by 't_end', 'break' by 'f_break'.
    """
    for (type, content, comment) in items:
        if type == 'other' and content == 'end':
            yield (type, 't_end', comment)
            continue
        elif type == 'other' and content == 'break':
            yield (type, 'f_break', comment)
            continue
        yield (type, content, comment)

# FIXME: re-implement xjoin2 here


_filter_begin = [reader, lexer, splitsemi, parser, grouper]
_cfilter_inner = [canonregs,
                  crenameregs,

                  cdetectregs,
                  cmarkused,
                  # creplsave,
                  # creplrestore,

                  # FIXME: is this useful?
                  #fillannulnop,

                  decode,
                  makedataflow(addswch = False, drainargs = True),

                  zerog0,
                  cphyregs]
_filter_inner = [canonregs,
                 regextract,
                 renameregs,

                 replaceret,
                 munchret,
                 # killrestore,
                 xsplit,
                 killrasave,
                 avoidframespills,

                 detectregs,
                 killspadjusts,

                 #replsave,
                 
                 findlabels,
                 killunusedlabels,

                 protectallcallregs,
                 initspfp,

                 xjoin1,

                 decode,

                 markused,
                 markaligned,

                 makespecialtransform('tlsp', regmagic),
                 makespecialtransform('fp', regmagic),
                 makespecialtransform('ra', regmagic),
                 makespecialtransform('ra_leaf', regmagic),
                 renamelocals,

                 compress,

                 xjoin2,

                 #fillannulnop,

                 decode,
                 makedataflow(addswch = False, drainargs = True),

                 markdelay,
                 #protectend,
                 replacebreakend,
                 
                 killnoregs,
                 
                 zerog0,
                 phyregs]
_filter_end = [ctlbits, flattener, printer]


from ...common.asmproc import fixup

def filter(*args):
    return fixup.filter(_filter_begin, _filter_inner, _cfilter_inner, _filter_end, *args)
