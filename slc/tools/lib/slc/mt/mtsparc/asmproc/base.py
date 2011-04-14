import re
from decode import decode
from ..regdefs import regmagic
from ....msg import die

_spreg = regmagic.legacy_to_canon('sp')
_fpreg = regmagic.legacy_to_canon('fp')

_re_call = re.compile(r'call\s')
_re_saverest = re.compile(r'(save|restore)\s')
def detectregs(fundata, items):
    """
    Check if FP/SP are used and whether there are calls.

    SP is assumed to be needed in presence of a function call,
    regardless of whether it is used in the current function body.
    """

    use_sp = False
    use_fp = False
    hasjumps = False
    spreg = regmagic.alias_to_vname('tlsp')
    fpreg = regmagic.alias_to_vname('fp')
    for (type, content, comment) in items:
        if type == 'other':
            if _re_call.match(content) is not None:
                hasjumps = True
                use_sp = True
            if spreg in content and not content.startswith('save') and not content.startswith('restore'):
                use_sp = True
            if fpreg in content and not content.startswith('save') and not content.startswith('restore'):
                use_fp = True
        yield (type, content, comment)
    fundata['use_sp'] = use_sp
    fundata['use_fp'] = use_fp
    fundata['hasjumps'] = hasjumps

def cdetectregs(fundata, items):
    """
    Check if save/restore is used and whether there are calls.
    """
    use_saverestore = True
    hasjumps = False
    for (type, content, comment) in items:
        if type == 'other':
            if _re_saverest.match(content) is not None:
                use_saverestore = True
            elif _re_call.match(content) is not None:
                hasjumps = True
        yield (type, content, comment)
    fundata['use_saverestore'] = use_saverestore
    fundata['hasjumps'] = hasjumps

# _re_sr = re.compile(r'(?:save|restore)\s')
# def detectleaf(fundata, items):
#     """
#     We need to know this to decide whether we have a
#     leaf function. 

#     Depending on the result, the name of SP and FP change too.
#     """
#     isleaf = True
#     for (type, content, comment) in items:
#         if type == 'other':
#             m = _re_sr.match(content)
#             if m is not None:
#                 isleaf = False
#         yield (type, content, comment)
#     fundata['isleaf'] = isleaf

def killrestore(fundata, items):
    for (type, content, comment) in items:
        if type == 'other' and content.startswith('restore'):
            yield ('empty','','MT: killed ' + content)
            continue
        yield (type, content, comment)

def replsave(fundata, items):
    spreg = regmagic.alias_to_vname('tlsp')
    for (type, content, comment) in items:
        if type == 'other' and content.startswith('save'):
            if not fundata['use_sp'] and spreg in content:
                # SP is not used in the body other than this save
                # (it was generated because of some shared argument using a local register)
                # so just kill it
                yield ('emty','','MT: killed ' + content)
                continue
            rest = content[4:].strip()
            if ',' in rest:
                yield ('other', 'add ' + rest, 'MT: save')
            else:
                yield ('emty','','MT: killed ' + content)
            continue
        yield (type, content, comment)
                


def renamelocals(fundata, items):
    """
    On the SPARC the MT locals are not mapped in allocation order, to
    preserve some ins/reg semantics in save/restore and calls.
    However if there are no calls, anything above the first 8 (the first 8 are used for MT arg passing)
    can be renamed.
    """

    rmask = fundata['usedregs']
    renamings = []
    for nr in xrange(8, 31):
        regname = '$l%d' % nr
        newnr = nr
        if rmask[nr] == True and False in rmask[:nr] and not fundata['hasjumps']:
            freenr = [i for (i,r) in enumerate(rmask) if r == False][0]
            newnr = freenr
            newname = '$l%d' % newnr
            rmask[nr] = False
            rmask[newnr] = True
            renamings.append((regname, newname))
            yield ('empty','','MT: reg %s renamed to %s' % (regname, newname))

    dorename = len(renamings) > 0

    for (type, content, comment) in items:
        if type == 'other' and dorename:
            for (old,new) in renamings:
                content = content.replace(old, new)
        yield (type, content, comment)

def replaceret(fundata, items):
    """
    Replace returns by 'end'. Remove the delay slot as well, since
    there is no work left to do in a thread function after 'end'.
    """
    ra1 = regmagic.alias_to_vname('ra')
    ra2 = regmagic.alias_to_vname('ra_leaf')
    restr = r'retl?|jmpl?\s+(?:\$%s|\$%s)\s*\+\s*8' % (ra1[1:], ra2[1:])
    #print 'YYY', ra1, ra2, restr
    retre = re.compile(restr)
    skipnext = False
    for (type, content, comment) in items:
        if type == 'other':
            m = retre.match(content)
            if m is not None:
                assert skipnext == False # ret in delay slot of ret?
                skipnext = True
                yield ('other','end', 'MT: end thread')
                continue
            if skipnext:
                yield ('empty', '', 'MT: killed ' + content)
                skipnext = False
                continue
        elif type == 'label':
            assert skipnext == False # label in delay slot?
        
        yield (type, content, comment)
        
def xsplit(fundata, items):
    """
    Cut the prologue from the function body.
    """

    name = fundata['name']
    lbl = '__slf_%s:' % name
    szdir = '.size\t__slf_%s' % name

    for (type, content, comment) in items:
        if content.startswith('.type') \
                or content.startswith('.proc') \
                or content.startswith('.global') \
                or content.startswith(szdir):
            continue
        elif type == 'label' and content == lbl:
            continue
        yield (type, content, comment)

    fundata['prologue'] = []


def initsp(fundata, items):
    """
    If SP is needed, initialize it in the prologue.
    """
    spreg = regmagic.alias_to_vname("tlsp")
    if fundata['use_sp']:
        if not initsp.extra_options.get('tls-via-gettid', False):
            fundata['prologue'].insert(0, ('other', 'ldfp %s' % spreg, 'MT: init SP'))
        else:
            fundata['prologue'] = [
                ('other', 'gettid %s' % spreg, 'MT: init SP'),
                ('other', 'sethi %hi(__first_tls_top), $l1', 'MT: init SP'),
                ('other', 'sll %s,10,%s' % (spreg,spreg), 'MT: init SP'),
                ('other', 'add %s,$l1,%s' % (spreg,spreg), 'MT: init SP'),
                ] + fundata['prologue']

    return items

def xjoin1(fundata, items):
    """
    Rebuild the function code back into a single body.
    """
    for n in fundata['prologue']:
        yield n
    yield ('empty', '', 'MTREG_SET')
    for n in items:
        yield n


def xjoin2(fundata, items):
    """
    Add function header and footer.
    """

    name = fundata['name']
    regs = fundata['regs']

    if fundata['global']:
        yield ('directive', '.global\t%s' % name, '')
    yield ('directive', '.type\t%s, #function' % name, '')
    yield ('directive', '.registers\t%d %d %d %d %d %d' % tuple(regs), '')
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
    


_presets = set(["$l0"])
def grouper(items):
    """
    Group function code together (when applicable).
    Produce a generator of (type, code, comment).

    For functions, emit a single item with type == "fun". In this case
    code is a dictionary with keys:
    - "name" : name of the function
    - "body" : stream of instructions
    """
    
    state = 0
    queue = []
    name = None
    globl = False

    for (type, content, comment) in items:
        if state == 0 and content.startswith('.global'):
            name = content.split(' ')[1]
            queue.append((type, content, comment))
            state = 1
            globl = True
            continue
        if state in [0, 1] and content.startswith('.type') and content.endswith('#function'):
            maybe_name = content.split(None)[1].split(',')[0]
            if state == 1 and maybe_name != name:
                # .global seen before, with different name
                # so reinit.
                globl = False
                for q in queue: yield q
                queue = []
            name = maybe_name
            queue.append((type, content, comment))
            state = 2
            continue
        if state == 2:
            # expect: stuff, then .size
            queue.append((type, content, comment))
            if (not content.startswith('.size')) or content.split(None)[1].split(',')[0] != name:
                continue
            state = 0
            if name.startswith('__slf_'):
                funtype = 'fun'
                name = name[6:]
            else:
                funtype = 'cfun'
            yield (funtype, {'name':name,
                             'body':queue,
                             'global':globl,
                             'specials':{},
                             'presets':_presets.copy()}, '')
            globl = False
            queue = []
            continue

        for q in queue:
            yield q
        queue = []
        state = 0
        yield (type, content, comment)



_g1reg = regmagic.legacy_to_canon('g1')
_iregs = [regmagic.legacy_to_canon('i%d' % i) for i in xrange(0,8)]
_oregs = [regmagic.legacy_to_canon('o%d' % i) for i in xrange(0,8)]

_re_canon = re.compile(r'(\$\d+)')
def cmarkused(fundata, items):
    """
    Collect which canonical registers are currently in use
    the function body.
    """
    riset = set()

    if fundata['use_saverestore']:
        riset.add(_iregs[6])
        riset.add(_iregs[7])

    for (type, content, comment) in items:
        if type == 'other':
            for rm in _re_canon.finditer(content):
                r = rm.group(1)
                if r in _iregs:
                    riset.add(r)
        yield (type, content, comment)
    fundata['usediregs'] = riset

_trivsave = re.compile(r'save\s*$')
_save = re.compile(r'save\s+([^,]+),([^,]+),([^,]+)$')
_trivrestore = re.compile(r'restore\s*$')
_restore = re.compile(r'restore\s+([^,]+),([^,]+),([^,]+)$')


def creplsave(fundata, items):

    riset = fundata['usediregs']

    for (type, content, comment) in items:
        found = False
        m = _save.match(content)
        if m is not None:
            found = True
            A = m.group(1).strip()
            B = m.group(2).strip()
            C = m.group(3).strip()

            if B.startswith('-'):
                addinsn = 'sub'
                B = B[1:]
            else:
                addinsn = 'add'

        elif _trivsave.match(content) is not None:
            found = True
            A = B = C = None
            addinsn = None

        if found:
            # found a save, replace
            if A is not None:
                yield ('other', '%s %s, %s, %s' % (addinsn, A, B, _g1reg), 'MT: save')
            for i in xrange(0, 8, 2):
                ireg = _iregs[i]
                ireg2 = _iregs[i+1]
                if ireg in riset and ireg2 in riset: # or oreg in roset or oreg2 in roset:
                    yield ('other', 'std %s, [%s + %d]' % (ireg, _spreg, i*4 + 32), 'MT: save')
                elif ireg in riset:
                    yield ('other', 'st %s, [%s + %d]' % (ireg, _spreg, i*4 + 32), 'MT: save')
                elif ireg2 in riset:
                    yield ('other', 'st %s, [%s + %d]' % (ireg2, _spreg, (i+1)*4 + 32), 'MT: save')
            for i in xrange(0, 8):
                ireg = _iregs[i]
                if ireg in riset:
                    yield ('other','mov %s, %s' % (_oregs[i], ireg), 'MT: save')
            if A is not None:
                yield ('other', 'mov %s, %s' % (_g1reg, C), 'MT: save')
        else:
            yield (type, content, comment)


def creplrestore(fundata, items):

    riset = fundata['usediregs']

    for (type, content, comment) in items:
        found = False
        m = _restore.match(content)
        if m is not None:
            found = True
            A = m.group(1).strip()
            B = m.group(2).strip()
            C = m.group(3).strip()

            if B.startswith('-'):
                addinsn = 'sub'
                B = B[1:]
            else:
                addinsn = 'add'

        elif _trivrestore.match(content) is not None:
            found = True
            A = B = C = None
            addinsn = None

        if found:
            # found a save, replace# FIXME: restore
            if A is not None:
                yield ('other', '%s %s, %s, %s' % (addinsn, A, B, _g1reg), 'MT: restore')
            for i in xrange(7, -1, -1):
                ireg = _iregs[i]
                if ireg in riset:
                    yield ('other', 'mov %s, %s' % (ireg, _oregs[i]), 'MT: restore')
                if i % 2 == 0:
                    ireg2 = _iregs[i+1]
                    if ireg in riset and ireg2 in riset:
                        yield ('other', 'ldd [%s + %d], %s' % (_spreg, i * 4 + 32, ireg), 'MT: restore')
                    elif ireg in riset:
                        yield ('other', 'ld [%s + %d], %s' % (_spreg, i * 4 + 32, ireg), 'MT: restore')
                    elif ireg2 in riset:
                        yield ('other', 'ld [%s + %d], %s' % (_spreg, (i+1) * 4 + 32, ireg2), 'MT: restore')
            if A is not None:
                yield ('other', 'mov %s, %s' % (_g1regs, C), 'MT: restore')
        else:
            yield (type, content, comment)

_ivregs = [regmagic.get_vregs_for_legacy('i%d' % i)[0] for i in xrange(0,8)]
_ovregs = [regmagic.get_vregs_for_legacy('o%d' % i)[0] for i in xrange(0,8)]

def protectallcallregs(fundata, items):
    """
    Prime the i/o registers in presence of a call (they
    may be read by the callee)
    """
    if fundata['hasjumps']:
        for i in xrange(0, 8):
            if i != 6: # avoid setting FP/SP here
                fundata['prologue'].append(('other', 'clr $%s' % _ivregs[i]['name'], 'maybe used in call'))
        for i in xrange(0, 6):
            fundata['prologue'].append(('other', 'clr $%s' % _ovregs[i]['name'], 'maybe used in call'))
    return items


def protectend(fundata, items):
    """
    Insert a nop before "end" if end is placed after
    a delayed instruction.
    """
    indelayslot = False
    for (type, content, comment) in items:
        if type == 'other':
            if 'DELAYSLOT' in comment:
                indelayslot = True
            else:
                if indelayslot == True and content == 'end':
                    yield ('other', 'nop', 'MT: end after delay slot')
                indelayslot = False
        yield (type, content, comment)
        
    


from common import *
from ...common.asmproc.regextract import *
from ...common.asmproc.renameregs import *

def crenameregs(fundata, items):
    return crenameregs_gen(fundata, items, regmagic)
def renameregs(fundata, items):
    return renameregs_gen(fundata, items, regmagic)


def zerog0(fundata, items):
    for (type, content, comment) in items:
        content = content.replace('$31','%r0').replace('$l31','%r0')
        yield (type, content, comment)

from ...common.asmproc.labels import *
from ...common.asmproc.markused import *
from ...common.asmproc.compress import *
from delay import *

_filter_begin = [reader, lexer, splitsemi, parser, grouper]
_cfilter_inner = [canonregs,
                  crenameregs,

                  cdetectregs,
                  cmarkused,
                  creplsave,
                  creplrestore,


                  zerog0,
                  cphyregs]
_filter_inner = [canonregs,
                 regextract,
                 renameregs,

                 replaceret,
                 killrestore,
                 detectregs,

                 replsave,
                 
                 findlabels,
                 killunusedlabels,

                 xsplit,

                 protectallcallregs,
                 initsp,


                 xjoin1,

                 markused,

                 makespecialtransform('tlsp', regmagic),
                 makespecialtransform('fp', regmagic),
                 makespecialtransform('ra', regmagic),
                 makespecialtransform('ra_leaf', regmagic),
                 renamelocals,

                 compress,

                 xjoin2,

                 decode,
                 markdelay,
                 protectend,

                 zerog0,
                 phyregs]
_filter_end = [flattener, printer]


from ...common.asmproc import fixup

def filter(*args):
    return fixup.filter(_filter_begin, _filter_inner, _cfilter_inner, _filter_end, *args)
