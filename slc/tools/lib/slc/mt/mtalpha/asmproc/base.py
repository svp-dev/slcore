import re
from ..regdefs import regmagic

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
        
        if state == 0 and content.startswith('.globl'):
            name = content.split(' ')[1]
            queue.append((type, content, comment))               
            state = 1
            globl = True
            continue
        if state in [0, 1] and content.startswith('.ent'):
            maybe_name = content.split(' ')[1]
            if state == 1 and maybe_name != name:
                # globl seen before, but for a different name;
                # it must be for a different declaration, so reinit
                globl = False
                for q in queue: yield q
                queue = []

            name = maybe_name
            queue.append((type, content, comment))               
            state = 2
            continue
        if state == 2:    
            # expect: stuff, then .end XXX...
            queue.append((type, content, comment))
            if not content.startswith('.end'):
                continue
            maybe_name = content.split(' ')[1]
            assert maybe_name == name # cannot have .end without .ent

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
                             'prologue':[], 
                             'pprologue':[],
                             'prologuen':0,
                             'presets':_presets.copy()},'')
            globl = False
            queue = []
            continue

        for q in queue:
            yield q
        queue = []
        state = 0
        yield (type, content, comment)



def stripmask(fundata, items):
    """
    Remove .mask directives, unsupported for now
    """

    for (type, content, comment) in items:
        if type == 'directive' and content.startswith('.mask'):
            yield ('empty','','MT: unsupported %s' % content)
        else:
            yield (type, content, comment)
            
from ...common.asmproc.renameregs import *

def renameregs(fundata, items):
    return renameregs_gen(fundata, items, regmagic)
def crenameregs(fundata, items):
    return crenameregs_gen(fundata, items, regmagic)

_re_jsr = re.compile(r'jsr\s|bs?r\s.*!samegp')
def detectgp(fundata, items):
    """
    Check if GP is used; whether they are needed impacts
    later stages.
    """
    use_gp = 0
    gpreg = regmagic.alias_to_vname('gp')
    for (type, content, comment) in items:
        if type == 'other':
            if gpreg in content:
                use_gp = 1
        yield (type, content, comment)
    for (type, content, comment) in fundata['prologue'] + fundata['pprologue']:
        if type == 'other':
            if gpreg in content:
                use_gp = 1
    fundata['use_gp'] = use_gp


_re_regs = re.compile(r'\$[gsld]f?\d+')
def detectsp(fundata, items):
    """
    Check if SP is used; whether it is needed impacts
    later stages.

    SP is assumed to be needed in presence of a function call,
    regardless of whether it is used in the current function body. However
    SP-to-SP arithmetic does not count.
    """
    use_sp = 0
    spreg = regmagic.alias_to_vname('tlsp')
    use_gp = 0
    for (type, content, comment) in items:
        if type == 'other':
            if _re_jsr.match(content) is not None:
                use_sp = 1
            else:
                allregs = _re_regs.findall(content)
                #print "XX", content, allregs
                if spreg in allregs:
                    others = [x for x in allregs if x != spreg]
                    if len(others) != 0:
                        # some other reg than SP involved, assume needed
                        use_sp = 1
                    else:
                        # mark for killspadjusts below
                        comment = comment + ' SP_ADJUST'
        yield (type, content, comment)

    p = []
    for (type, content, comment) in fundata['prologue']:
        if type == 'other':
            if _re_jsr.match(content) is not None:
                use_sp = 1
            else:
                allregs = _re_regs.findall(content)
                #print "XX", content, allregs
                if spreg in allregs:
                    others = [x for x in allregs if x != spreg]
                    if len(others) != 0:
                        # some other reg than SP involved, assume needed
                        use_sp = 1
                    else:
                        # mark for killspadjusts below
                        comment = comment + ' SP_ADJUST'
        p.append((type, content, comment))
    fundata['prologue'] = p

    p = []
    for (type, content, comment) in fundata['pprologue']:
        if type == 'other':
            if _re_jsr.match(content) is not None:
                use_sp = 1
            else:
                allregs = _re_regs.findall(content)
                #print "XX", content, allregs
                if spreg in allregs:
                    others = [x for x in allregs if x != spreg]
                    if len(others) != 0:
                        # some other reg than SP involved, assume needed
                        use_sp = 1
                    else:
                        # mark for killspadjusts below
                        comment = comment + ' SP_ADJUST'
        p.append((type, content, comment))
    fundata['pprologue'] = p


    fundata['use_sp'] = use_sp

def killspadjusts(fundata, items):
    """
    Remove SP adjustments if SP is not needed.
    """
    usesp = fundata['use_sp']

    for (type, content, comment) in items:
        if not usesp and type == 'other' and ' SP_ADJUST' in comment:
            yield ('empty','','MT: killed sp adjust: ' + content)
            continue
        yield (type, content, comment)

    p = []
    for (type, content, comment) in fundata['prologue']:
        if not usesp and type == 'other' and ' SP_ADJUST' in comment:
            p.append(('empty','','MT: killed sp adjust: ' + content))
            continue
        p.append((type, content, comment))
    fundata['prologue'] = p

    p = []
    for (type, content, comment) in fundata['pprologue']:
        if not usesp and type == 'other' and ' SP_ADJUST' in comment:
            p.append(('empty','','MT: killed sp adjust: ' + content))
            continue
        p.append((type, content, comment))
    fundata['pprologue'] = p
    


def replaceret(fundata, items):
    """
    Replace 'ret' by 'end'.

    This may produce the 'end' annotation after a label. A
    later stage will introduce the necessary 'nop' instructions.
    """
    for (type, content, comment) in items:
        if type == 'other' and content.startswith('ret '):
            yield ('other','end','MT: end thread')
        else:
            yield (type, content, comment)

_re_munchret = re.compile(r'(ld.|ldah|mov|nop|fnop|unop)\s+|(bis|cpys)\s+\$lf?31,\$lf?31,\$lf?31')
_re_gpdisp = re.compile(r'(?:ldah?)\s+\$l\d+,.*\s+!gpdisp!(\d+)$') 

def munchret(fundata, items):
    """
    Remove possible frame adjustments before 'end'.
    """
    rekill = _re_munchret
    regpdisp = _re_gpdisp

    queue = []
    s = fundata.get('killed_gpdisp', set())
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
                m = regpdisp.match(content)
                if m is not None:
                    gpd = int(m.group(1))
                    s.add(gpd)
                queue.append(('empty','','MT: killed cleanup: %s' % content))
            for r in qpreserve:
                queue.append(r)
            queue.append(('other','end','MT: end thread'))
        else:
            queue.append((type, content, comment))

    fundata['killed_gpdisp'] = s
    return queue

def killgpdisp(fundata, items):
    """
    Remove all GP reloads previously tagged by munchret.
    """

    regpdisp = _re_gpdisp
    s = fundata["killed_gpdisp"]
    for (type, content, comment) in items:
        if type == 'other':
            m = regpdisp.match(content)
            if m is not None and int(m.group(1)) in s:
                yield ('empty', '', 'MT: killed gpreload: %s' % content)
                continue
        yield (type, content, comment)

def xsplit(fundata, items):
    """
    Extract the pre-prologue and prologue from the function body.

    They are identified as follows:

    __slf_NAME:   
         stuff    # pre-prologue
         stuff    # pre-prologue
         stuff    # pre-prologue
    __slf_NAME..ng:
         stuff    # prologue
         stuff    # prologue
         stuff    # prologue
         .prologue
         rest...
    """
    name = fundata['name']
    inprologue = 0
    inpprologue = 0

    lblng = '$__slf_%s..ng:' % name
    lbl = '__slf_%s:' % name

    for (type, content, comment) in items:
        if content.startswith('.frame ') \
                 or content.startswith('.mask ') \
                 or content.startswith('.fmask ') \
                 or content.startswith('.globl ') \
                 or content.startswith('.ent ') \
                 or content.startswith('.end '):
            continue
        elif type == 'label' and content == lblng:
            inpprologue = 0
            inprologue = 1
            continue
        elif inprologue == 1:
            if type == 'label' and content == lbl:
                continue
            elif content.startswith('.prologue'):
                fundata['prologuen'] = int(content.split(' ')[1])
                inprologue = 0
                continue
            fundata['prologue'].append((type, content, comment))
            continue
        elif type == 'label' and content == lbl:
            inpprologue = 1
            continue
        elif inpprologue == 1:
            if type == 'label':
                continue
            fundata['pprologue'].append((type, content, comment))
            continue
        
        yield (type, content, comment)

from ...common.asmproc.markused import *


def markused_specials(fundata, items):
    rmask = fundata['usedregs']
    spreg = regmagic.get_vreg('tlsp')['nr']
    gpreg = regmagic.get_vreg('gp')['nr']
    rareg = regmagic.get_vreg('ra')['nr']
    for (type, content, comment) in items:
        if type == 'other':
            if _re_jsr.match(content):
                # ensure specials are there for calls
                rmask[spreg] = True
                rmask[gpreg] = True # FIXME: no need for GP when we remove the callgate
                rmask[rareg] = True
        yield (type, content, comment)
    fundata['usedregs'] = rmask


def findcalls(fundata, items):
    """
    Detect if there are function calls.
    """
    hasjumps = False
    for (type, content, comment) in items:
        if type == "other" and _re_jsr.match(content):
            hasjumps = True
        yield (type, content, comment)
    fundata['hasjumps'] = hasjumps

_re_fspill = re.compile('st. \$lf?\d+')
def avoidframespills(fundata, items):
    """
    Remove spills of "protected" registers inserted
    in the prologue of the thread function.

    FIXME: revisit when function calls are within the same context?
    """
    p = []
    for (type, content, comment) in fundata['prologue']:
        if type == 'other' and _re_fspill.match(content) is not None:
            p.append(('empty','','MT: killed frame spill: %s' % content))
            continue
        p.append((type, content, comment))
    fundata['prologue'] = p
    return items

def initspecial(fundata, items):
    """
    If GP or SP are needed, initialize them in the prologue.
    """
    gli, shi, li, glf, shf, lf = fundata['regs']
    assert li > 3
    gpreg = regmagic.alias_to_vname("gp")
    spreg = regmagic.alias_to_vname("tlsp")
    pvreg = regmagic.alias_to_vname("pv")
    if fundata['hasjumps']:
        gpinitreg = pvreg
    else:
        gpinitreg = gpreg
    pprologue = fundata['pprologue']
    if fundata['use_gp']:
        # GP needs to be initialized, do it:
        newpprologue = []
        if len(pprologue) == 1:
            # form 1: "ldgp" was used by the compiler
            assert pprologue[0][1].startswith('ldgp')
            newpprologue.append(('other','ldpc %s' % gpinitreg,''))
            newpprologue.append(('other','ldgp %s, 0(%s)' % (gpreg, gpinitreg),''))
        else:
            #pprint.pprint(pprologue)
            assert (len(pprologue) == 2 and 'gpdisp' in pprologue[0][1] and 'gpdisp' in pprologue[1][1])
            disp = pprologue[0][1].split('!')[2]
            newpprologue.append(('other','ldpc %s' % gpinitreg,''))
            newpprologue.append(('other','ldah %s, 0(%s) !gpdisp!%s' % (gpreg, gpinitreg, disp),''))
            newpprologue.append(('other','lda %s, 0(%s) !gpdisp!%s' % (gpreg, gpreg, disp),''))
        fundata['pprologue'] = newpprologue
    else:
        assert len(pprologue) == 0
    #if fundata['use_pv']:
    #    fundata['prologue'].insert(0, ('other','mov $g%d, $l%d' % (pvargreg, pvreg), 'MT: init PV'))
    if fundata['use_sp']: # and spreg != 1: # FIXME: why != 1?
        fundata['prologue'].insert(0, ('other','ldfp %s' % spreg,'MT: init SP'))

    return items

def killgpreload(fundata, items):
    """
    Remove GP reloads after function calls.
    
    FIXME: valid only when using the callgate; revisit when doing function calls
    in the same context.
    """
    for (type, content, comment) in items:
        if type == 'other' and '!gpdisp!' in content:
            yield ('empty', '', 'MT: killed GP reload: %s' % content)
        else:
            yield (type, content, comment)

def optprologue(fundata, items):
    """
    1) Remove spills of RA and FP in prologue; and
    2) initialize FP if it is used in the body.

    FIXME: task #1 is valid only with the callgate; need to revisit?
    """
    fpreg = regmagic.alias_to_vname('fp')
    rareg = regmagic.alias_to_vname('ra')
    found = False
    for (type, content, comment) in items:
        if type == 'other' and fpreg in content:
            found = True
        yield (type, content, comment)

    # 2009-04-02: beware of not fidgeting too much with FP!
    newprologue = []
    for (type, content, comment) in fundata['prologue']:
        if type == 'other' and (rareg in content or content.startswith("stq %s," % fpreg)):
            newprologue.append(('empty','','MT: FP spill: %s' % content))
        else:
            newprologue.append((type, content, comment))

    if found or fundata['hasjumps']:
        newprologue.insert(0,('other','mov $l31, %s' % fpreg,'MT: init FP'))
    fundata['prologue'] = newprologue

def findusedcallregs(fundata, items):
    """
    Check which registers that may be used for passing
    arguments in function calls are actually used in the
    body.

    (they will need special treatment by protectcallregs)
    """
    tosave = []
    ftosave = []
    for areg in xrange(0,6):
        r = regmagic.get_vregs_for_legacy('a%d'%areg)
        rf = regmagic.get_vregs_for_legacy('fa%d'%areg)
        
        assert len(r) == 1 and r[0]['cat'] == 'l'
        assert len(rf) == 1 and rf[0]['cat'] == 'l'

        regnr = r[0]['nr'] 
        fregnr = rf[0]['nr'] 

        if fundata['usedregs'][regnr] or fundata['usedfregs'][fregnr]:
            if fundata['usedregs'][regnr]:
                tosave.append(regnr)
            else:
                tosave.append(None)
            if fundata['usedfregs'][fregnr]:
                ftosave.append(fregnr)
            else:
                ftosave.append(None)
        else: 
            break

    # print "FOO", fundata['name'], tosave, ftosave
    fundata['usedcallregs'] = tosave
    fundata['usedcallfregs'] = ftosave
    return items

# def findcallspills(self, fundata, items):
#    sp = self.reg_aliases['tlsp']
#    respill = r'st[qt]\s+\$l(f?\d+)\s*,\s*\d+\(\$%s\)' % sp
#    respill = re.compile(respill)
#    tosave = set()
#    ftosave = set()
#    for (type, content, comment) in items:
#       if type == 'other':
#          m = respill.match(content)
#          if m is not None:
#             reg = m.group(1)
#             if reg[0] == 'f':
#                ftosave.add(int(reg[1:]))
#             else:
#                tosave.add(int(reg))
#       yield (type, content, comment)
#    fundata['spilledcallregs'] = tosave
#    fundata['spilledcallfregs'] = ftosave

def protectcallregs(fundata, items):
    """
    If there are function calls, then all argument registers
    that are actually used need to be initialized.
    """
    doprotect = protectcallregs.extra_options.get('clear-local-regs',False)
    if doprotect and fundata['hasjumps']:
        init = []
        for r in set(fundata['usedcallregs']): # | fundata['spilledcallregs']:
            if r is not None:
                init.append(('other','clr $l%d' % r, 'init call reg'))
        for r in set(fundata['usedcallfregs']): # | fundata['spilledcallfregs']:
            if r is not None:
                init.append(('other','fclr $lf%d' % r, 'init call freg'))
        fundata['prologue'] = fundata['prologue'] + init
    return items

_re_rwj_jsr =  re.compile('jsr\s+\$l(\d+)\s*,\s*\(\s*\$l(\d+)\s*\)')
_re_rwj_brre = re.compile('br\s+\$l31\s*,\s*(\S+)\s*!samegp')
_re_rwj_bsrre = re.compile('bsr\s+\$l(\d+)\s*,\s*(\S+)\s*!samegp')
def rewritejsr(fundata, items):
    """
    Replace function calls by a create to the callgate.
    """
    spreg = regmagic.get_vreg('tlsp')['nr']
    gpreg = regmagic.get_vreg('gp')['nr']
    rareg = regmagic.get_vreg('ra')['nr']
    pvreg = regmagic.get_vreg('pv')['nr']
    rvreg = regmagic.get_vreg('rv')['nr']
    frvreg = regmagic.get_vreg('frv')['nr']

    rvused = fundata['usedregs'][rvreg]
    frvused = fundata['usedfregs'][frvreg]
    usedcallregs = fundata['usedcallregs']
    usedcallfregs = fundata['usedcallfregs']
    protocol = max(len(usedcallregs),len(usedcallfregs))
    # struct { protocol/retval, fptr/fretval, args... }
    gate_struct_size = 8 + 8 + 8 * 2 * protocol 


    jsrre = _re_rwj_jsr
    brre = _re_rwj_brre
    bsrre = _re_rwj_bsrre
    for (type, content, comment) in items:
        if type == "other":
            m = brre.match(content)
            if m is not None:
                # handle "BR"
                lbl = m.group(1)
                yield ('empty','','MT: br call begin: %s' % content)
                yield ('other','lda $l%d, %d($l31)' % (rareg, protocol), 'RA <- nr_call_regs')
                yield ('other','stq $l%d, -8($l%d)' % (rareg, spreg), '.protocol := RA (nr_call_regs)')
                yield ('other','allocate 8, $l%d' % rareg, 'RA <- fid, PID = default|suspend')
                yield ('other','ldah $l%d, %s($l%d) !gprelhigh' % (pvreg, lbl, gpreg), 'PV <- target')
                yield ('other','lda $l%d, %s($l%d) !gprellow' % (pvreg, lbl, pvreg), 'PV <- target')
                yield ('other','stq $l%d, -16($l%d)' % (pvreg, spreg), '.fptr := PV')
                arg_offset = -24
                for i in xrange(protocol):
                    if len(usedcallregs)>i and usedcallregs[i] is not None:
                        yield ('other', 'stq $l%d, %d($l%d)' % (usedcallregs[i], arg_offset, spreg), '.arg[%d]' % i)
                    if len(usedcallfregs)>i and usedcallfregs[i] is not None:
                        yield ('other', 'stt $lf%d, %d($l%d)' % (usedcallfregs[i], arg_offset - 8, spreg), 
                                 '.farg[%d]' % i)
                    arg_offset -= 16
                yield ('other','ldq $l%d, __sl_callgate($l%d) !literal' % (pvreg, gpreg), 'PV <- callgate')
                yield ('other','setstart $l%d, $l%d' % (rareg, spreg), 'setstart(SP)')
                yield ('other','lda $l%d, 1($l%d)' % (spreg, spreg), 'SP <= SP + 1')
                yield ('other','setlimit $l%d, $l%d' % (rareg, spreg), 'setlimit(SP+1)')
                yield ('other', 'wmb', '')
                yield ('other','crei $l%d, 0($l%d)' % (rareg, pvreg), 'start call')
                yield ('other','sync $l%d, $l%d' % (rareg, pvreg), 'SYNC: start wait')
                yield ('other','mov $l%d, $l31' % (pvreg), 'SYNC: wait for return')
                yield ('other','release $l%d' % rareg, 'SYNC: end')
                yield ('other','end','call is end of thread')
                yield ('empty','','MT: br call end')
                continue

            m = bsrre.match(content)
            if m is not None:
                # handle "BSR"
                ra = int(m.group(1))
                lbl = m.group(2)
                yield ('empty','','MT: bsr call begin: %s' % content)
                yield ('other','lda $l%d, %d($l31)' % (ra, protocol), 'RA <- nr_call_regs')
                yield ('other','stq $l%d, -8($l%d)' % (ra, spreg), '.protocol := RA (nr_call_regs)')
                yield ('other','allocate 8, $l%d' % ra, 'RA <- fid, PID = default|suspend')
                yield ('other','ldah $l%d, %s($l%d) !gprelhigh' % (pvreg, lbl, gpreg), 'PV <- target')
                yield ('other','lda $l%d, %s($l%d) !gprellow' % (pvreg, lbl, pvreg), 'PV <- target')
                yield ('other','stq $l%d, -16($l%d)' % (pvreg, spreg), '.fptr := PV')
                arg_offset = -24
                for i in xrange(protocol):
                    if len(usedcallregs)>i and usedcallregs[i] is not None:
                        yield ('other', 'stq $l%d, %d($l%d)' % (usedcallregs[i], arg_offset, spreg), '.arg[%d]' % i)
                    if len(usedcallfregs)>i and usedcallfregs[i] is not None:
                        yield ('other', 'stt $lf%d, %d($l%d)' % (usedcallfregs[i], arg_offset - 8, spreg), 
                                 '.farg[%d]' % i)
                    arg_offset -= 16
                yield ('other','ldq $l%d, __sl_callgate($l%d) !literal' % (pvreg, gpreg), 'PV <- callgate')
                yield ('other','setstart $l%d, $l%d' % (ra, spreg), 'setstart(SP)')
                yield ('other','lda $l%d, 1($l%d)' % (spreg, spreg), 'SP <= SP + 1')
                yield ('other','setlimit $l%d, $l%d' % (ra, spreg), 'setlimit(SP+1)')
                yield ('other', 'wmb', '')
                yield ('other','crei $l%d, 0($l%d)' % (ra, pvreg), 'start call')
                yield ('other','lda $l%d, -1($l%d)' % (spreg, spreg), 'SP <= SP - 1')
                yield ('other','sync $l%d, $l%d' % (ra, pvreg), 'SYNC: start wait')
                yield ('other','mov $l%d, $l31' % (pvreg), 'SYNC: wait for return')
                yield ('other','release $l%d' % rareg, 'SYNC: end')
                if rvused:
                    yield ('other','ldq $l%d, -8($l%d)' % (rvreg, spreg), 'restore RV')
                    yield ('other','mov $l%d, $l%d' % (rvreg, rvreg), 'complete memory load')
                if frvused:
                    yield ('other','ldt $lf%d, -16($l%d)' % (frvreg, spreg), 'restore FRV')
                    yield ('other','cpys $lf%d, $lf%d, $lf%d' % (frvreg, frvreg, frvreg), 'complete memory load')
                yield ('empty','','MT: bsr call end')
                continue

            m = jsrre.match(content)
            if m is not None:
                ra = int(m.group(1))
                pv = int(m.group(2))

                # compute structure
                yield ('empty','','MT: jsr call begin: %s' % content)
                yield ('other','lda $l%d, %d($l31)' % (ra, protocol), 'RA <- nr_call_regs')
                yield ('other','stq $l%d, -8($l%d)' % (ra, spreg), '.protocol := RA (nr_call_regs)')
                yield ('other','allocate 8, $l%d' % ra, 'RA <- fid, PID = default|suspend')
                yield ('other','stq $l%d, -16($l%d)' % (pv, spreg), '.fptr := PV')
                arg_offset = -24
                for i in xrange(protocol):
                    if len(usedcallregs)>i and usedcallregs[i] is not None:
                        yield ('other', 'stq $l%d, %d($l%d)' % (usedcallregs[i], arg_offset, spreg), '.arg[%d]' % i)
                    if len(usedcallfregs)>i and usedcallfregs[i] is not None:
                        yield ('other', 'stt $lf%d, %d($l%d)' % (usedcallfregs[i], arg_offset - 8, spreg), 
                                 '.farg[%d]' % i)
                    arg_offset -= 16
                yield ('other','ldq $l%d, __sl_callgate($l%d) !literal' % (pv, gpreg), 'PV <- callgate')
                yield ('other','setstart $l%d, $l%d' % (ra, spreg), 'setstart(SP)')
                yield ('other','lda $l%d, 1($l%d)' % (spreg, spreg), 'SP <= SP + 1')
                yield ('other','setlimit $l%d, $l%d' % (ra, spreg), 'setlimit(SP+1)')
                yield ('other', 'wmb', '')
                yield ('other','crei $l%d, 0($l%d)' % (ra, pv), 'start call')
                yield ('other','lda $l%d, -1($l%d)' % (spreg, spreg), 'SP <= SP - 1')
                yield ('other','sync $l%d, $l%d' % (ra, pvreg), 'SYNC: start wait')
                yield ('other','mov $l%d, $l31' % (pvreg), 'SYNC: wait for return')
                yield ('other','release $l%d' % ra, 'SYNC: end')
                if rvused:
                    yield ('other','ldq $l%d, -8($l%d)' % (rvreg, spreg), 'restore RV')
                    yield ('other','mov $l%d, $l%d' % (rvreg, rvreg), 'complete memory load')
                if frvused:
                    yield ('other','ldt $lf%d, -16($l%d)' % (frvreg, spreg), 'restore FRV')
                    yield ('other','cpys $lf%d, $lf%d, $lf%d' % (frvreg, frvreg, frvreg), 'complete memory load')
                yield ('empty','','MT: jsr call end')
                continue

            if content.startswith('jsr') or (content.startswith('b') and '!samegp' in content):
                die("%s: unsupported call: %s" % (fundata['name'],content))
            
        yield (type, content, comment)


def xjoin1(fundata, items):
    """
    Rebuild function code back into a single body.
    """
    name = fundata['name']
    for n in fundata['pprologue']:
        yield n
    yield ('label', '$%s..ng:' % name, '')
    for n in fundata['prologue']:
        yield n
    yield ('directive', '.prologue %d' % (fundata['prologuen']), '')
    yield ('empty', '', 'MTREG_SET')
    for n in items:
        yield n

def xjoin2(fundata, items):
    """
    Add function header.
    """
    name = fundata['name']
    regs = fundata['regs']
    if fundata['global']:
        yield ('directive', '.globl %s' % name, '')
    yield ('directive', '.ent %s' % name, '')
    yield ('directive', '.registers %d %d %d %d %d %d' % tuple(regs), '')
    yield ('label', '%s:' % name, '')
    if fundata['use_gp']:
        yield ('directive', '.base $l%d' % fundata['specials']['gp'], '')

    p = fundata['presets']
    # print p
    for (type, content, comment) in items:
        if type == 'empty' and comment == 'MTREG_SET':
            if fundata['use_gp']: yield ('empty','','MTREG_SPEC: $l%d' % fundata['specials']['gp'])
            if fundata['use_sp']: yield ('empty','','MTREG_SPEC: $l%d' % fundata['specials']['tlsp'])
            #if fundata['use_pv']: yield ('empty','','MTREG_SPEC: $l%d' % fundata['specials']['pv'])
            yield ('empty', '', 'MTREG_SET: %s' % ','.join(p))
        else:
            yield (type, content, comment)
    yield ('directive', '.end %s' % name, '')
    yield ('directive', '.base %s' % regmagic.get_legacy('gp'), '')


from ...common.asmproc.compress import *


def makedetectload(keyname, regname):
    """
    Is there any load of RA or FP from the stack?
    """
    def detectload(fundata, items):
        hasregload = False
        spreg = fundata['specials']['tlsp']
        spec = fundata['specials'][regname]
        r = re.compile(r'ldq\s+\$l%d\s*,\s*\d+\(\$l%d\)' % (spec, spreg))
        for (type, content, comment) in items:
            if type == "other":
                if r.match(content): hasregload = True
            yield (type, content, comment)

        fundata[keyname] = hasregload
    return detectload

def makecompleteload( keyname, regname):
    """
    Force a read on RA and FP before "end" if they were used.

    This is needed since they may be target of a memory load
    and the register context may be reused for another thread 
    (after the current thread terminates) before the load completes.
    """
    def completeload(fundata, items):
        hasload = fundata[keyname]
        regnr = fundata['specials'][regname]
        for (type, content, comment) in items:
            if type == 'other' and content == "end":
                if hasload:
                    yield ('other','mov $l%d, $l%d' % (regnr, regnr), 'maybe complete ldq')
            yield (type, content, comment)
    return completeload

def makeprotectspecial(keyname, regname):
    """
    If a special register is used in the function body, maybe
    being read from without initialization (e.g. RA), then
    clear it at the start.
    """
    # this transorm appears very late in the chain,
    # and here there is no more split with the prologue.
    # so we have to detect explicitly the prologue boundary again.
    def protectspecial(fundata, items):
        doprotect = protectspecial.extra_options.get('clear-local-regs',False)
        hasload = fundata[keyname]
        regnr = fundata['specials'][regname]
        for (type, content, comment) in items:
            if doprotect and content.startswith('.prologue') and hasload:
                yield ('other','clr $l%d' % regnr, 'init maybe used')
            yield (type, content, comment)
    return protectspecial

def protectallcallregs(fundata, items):
    doprotect = protectallcallregs.extra_options.get('clear-local-regs',False)
    if doprotect and fundata['hasjumps']:
        for leg_preg in regmagic.call_arg_registers:
            pregs = regmagic.get_vregs_for_legacy(leg_preg)
            assert len(pregs) == 1
            r = pregs[0]
            if r['species'] == 'f':
                pref = 'f'
            else:
                pref = ''
            fundata['prologue'].append(('other','%sclr $%s' % (pref, r['name']), 'maybe used in call'))

    return items

def protectcallsave(fundata, items):
    doprotect = protectcallsave.extra_options.get('clear-local-regs',False)
    if doprotect and fundata['hasjumps']:
        for leg_preg in regmagic.call_saved_registers:
            pregs = regmagic.get_vregs_for_legacy(leg_preg)
            assert len(pregs) == 1
            r = pregs[0]
            if r['species'] == 'f':
                pref = 'f'
            else:
                pref = ''
            fundata['prologue'].append(('other','%sclr $%s' % (pref, r['name']), 'maybe used in call'))

    return items
    
from common import *
from ...common.asmproc.regextract import *
from ...common.asmproc.renameregs import *

_filter_begin = [reader, lexer, splitsemi, parser, grouper]
_cfilter_inner = [canonregs, crenameregs, stripmask]
_filter_inner = [
                canonregs,
                regextract,
                renameregs,
                replaceret,
                munchret,
                killgpdisp,
                
                xsplit,
                
                avoidframespills,
                findcalls,
                protectcallsave,
                protectallcallregs,

                

                optprologue,

                detectgp,
                detectsp,
                killspadjusts,
                initspecial,

                #killgpreload,
                
                #markused,
                #findusedcallregs,
                #protectcallregs,
                #rewritejsr,
                
                xjoin1,
                
                markused,
                markused_specials,
                makespecialtransform('gp', regmagic),
                makespecialtransform('tlsp', regmagic),
                makespecialtransform('fp', regmagic),
                makespecialtransform('ra', regmagic,),
                makespecialtransform('pv', regmagic),
                
                compress,
                xjoin2,
                
                munchret,
                killgpdisp,
                #makeprotectspecial('hasjumps','fp'),
                makedetectload('hasraload','ra'),
                makecompleteload('hasraload','ra'),
                makeprotectspecial('hasraload', 'ra'),
                makedetectload('hasfpload','fp'),
                makecompleteload('hasfpload','fp'),
                stripmask,
                ]
_filter_end = [flattener, forcezero, printer]


from ...common.asmproc import fixup

def filter(*args):
    return fixup.filter(_filter_begin, _filter_inner, _cfilter_inner, _filter_end, *args)
