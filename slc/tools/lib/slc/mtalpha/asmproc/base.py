import re
from .. import regmagic
from ...front import opts, dump

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

_re_regs = re.compile(r'.registers (\d+) (\d+) (\d+) (\d+) (\d+) (\d+)')
def regextract(fundata, items):
    """
    Find the '.registers' directive in the function body,
    remove it from the code and cache the values for future stages.

    (the '.registers' directive is re-generated later by xjoin2)
    """
    for (type, content, comment) in items:
        m = _re_regs.match(content)
        if m is not None:
            assert not fundata.has_key('regs')
            fundata['regs'] = [int(x) for x in m.groups()]
            continue

        yield (type, content, comment)

_re_rr1reg = re.compile('(\$f?\d+)')
def renameregs(fundata, items):
    """
    Convert all legacy register names to their virtual
    counterparts. 
    """
    rd = fundata['regs']
    repl = regmagic.makerepl(rd[0],rd[1],rd[3],rd[4])
    for (type, content, comment) in items:
        content = _re_rr1reg.sub(repl, content)
        comment = _re_rr1reg.sub(repl, comment)
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
            

def crenameregs(fundata, items):
    """
    Collate legacy register names in regular C function
    bodies to the beginning of the register window.
    """
    repl = regmagic.makecrepl(fundata['name']
)
    for (type, content, comment) in items:
        content = _re_rr1reg.sub(repl, content)
        comment = _re_rr1reg.sub(repl, comment)
        yield (type, content, comment)

_re_jsr = re.compile(r'jsr\s|bs?r\s.*!samegp')
def detectregs(fundata, items):
    """
    Check if GP / SP are used; whether they are needed impacts
    later stages.

    SP is assumed to be needed in presence of a function call,
    regardless of whether it is used in the current function body.
    """
    use_sp = 0
    spreg = regmagic.alias_to_vname('tlsp')
    use_gp = 0
    gpreg = regmagic.alias_to_vname('gp')
    #use_pv = 0
    #pvreg = regmagic.alias_to_vname('pv')
    for (type, content, comment) in items:
        if type == 'other':
            if spreg in content or _re_jsr.match(content):
                use_sp = 1
            if gpreg in content:
                use_gp = 1
            #elif ('jsr' in content) and (pvreg in content):
            #    use_pv = 1
        yield (type, content, comment)
    fundata['use_sp'] = use_sp
    fundata['use_gp'] = use_gp
    #fundata['use_pv'] = use_pv

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
            for r in qkill:
                queue.append(('empty','','MT: killed cleanup: %s' % r[1]))
            for r in qpreserve:
                queue.append(r)
            queue.append(('other','end','MT: end thread'))
        else:
            queue.append((type, content, comment))

    return queue

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
    for (type, content, comment) in items:
        if content.startswith('.frame ') \
                 or content.startswith('.mask ') \
                 or content.startswith('.fmask ') \
                 or content.startswith('.globl ') \
                 or content.startswith('.ent ') \
                 or content.startswith('.end '):
            continue
        elif type == 'label' and content == ('$__slf_%s..ng:' % name):
            inpprologue = 0
            inprologue = 1
            continue
        elif inprologue == 1:
            if type == 'label' and content == ('__slf_%s:' % name):
                continue
            elif content.startswith('.prologue'):
                fundata['prologuen'] = int(content.split(' ')[1])
                inprologue = 0
                continue
            fundata['prologue'].append((type, content, comment))
            continue
        elif type == 'label' and content == ('__slf_%s:' % name):
            inpprologue = 1
            continue
        elif inpprologue == 1:
            if type == 'label':
                continue
            fundata['pprologue'].append((type, content, comment))
            continue
        
        yield (type, content, comment)

_re_li = re.compile(r'\$l(\d+)')
_re_lf = re.compile(r'\$lf(\d+)')
_re_gi = re.compile(r'\$g(\d+)')
_re_gf = re.compile(r'\$gf(\d+)')
def markused(fundata, items):
    """
    Collect which local and global registers are currently in use in
    the function body.
    """
    rmask = [False] * 32
    rfmask = [False] * 32
    rgmask = [False] * 32
    rgfmask = [False] * 32
    spreg = regmagic.get_vreg('tlsp')['nr']
    gpreg = regmagic.get_vreg('gp')['nr']
    rareg = regmagic.get_vreg('ra')['nr']
    for (type, content, comment) in items:
        if type == 'other':
            for rm in _re_li.finditer(content):
                n = int(rm.group(1))
                rmask[n] = True
            for rm in _re_lf.finditer(content):
                n = int(rm.group(1))
                rfmask[n] = True
            for rm in _re_gi.finditer(content):
                n = int(rm.group(1))
                rgmask[n] = True
            for rm in _re_gf.finditer(content):
                n = int(rm.group(1))
                rgfmask[n] = True
            if _re_jsr.match(content):
                # ensure specials are there for calls
                rmask[spreg] = True
                rmask[gpreg] = True # FIXME: no need for GP when we remove the callgate
                rmask[rareg] = True
        yield (type, content, comment)
    fundata['usedregs'] = rmask
    fundata['usedfregs'] = rfmask
    fundata['usedgl'] = rgmask
    fundata['usedglf'] = rgfmask

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
    if fundata['hasjumps']:
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

def makespecialtransform(special):
    """
    Create a filter that renames a special register
    to the first free lower position in the local window (if any).

    FIXME: revisit when function calls are performed within the same context
    (then renaming is not possible)
    """
    nr = regmagic.get_vreg(special)['nr']
    def dotransform(fundata, items):
        regname = '$l%d' % nr
        rmask = fundata['usedregs']
        dorename = False
        newnr = nr
        if rmask[nr] == True and False in rmask[:nr] and not fundata['hasjumps']:
            freenr = [i for (i,r) in enumerate(rmask) if r == False][0]
            newnr = freenr
            dorename = True
            newname = '$l%d' % freenr
            rmask[nr] = False
            rmask[freenr] = True
            yield ('empty','','MT: special "%s" renamed from %s to %s' % (special, regname, newname))
        fundata['specials'][special] = newnr
        for (type, content, comment) in items:
            if type == 'other' and dorename:
                content = content.replace(regname, newname)
            yield (type, content, comment)
    return dotransform


def compress(fundata, items):
    """
    Compress the local and global register
    windows based on which registers are actually used.

    FIXME: need to revisit when function calls are done locally.
    """

    regs = fundata['regs']
    rmask = fundata['usedregs']
    rfmask = fundata['usedfregs']
    rgmask = fundata['usedgl']
    rgfmask = fundata['usedglf']

    if not fundata['hasjumps']:
        # Compress integer locals
        lastfound = None
        for i in xrange(30, -1, -1):
            if rmask[i]:
                lastfound = i
                break
        if lastfound is None:
            newnr = 0
        else:
            newnr = lastfound + 1

        if newnr < regs[2]:
            yield ('empty','','MT: compressed integer locals from %d to %d' % (regs[2], newnr))
            regs[2] = newnr

    # Compress integer globals
    lastfound = None
    for i in xrange(30, -1, -1):
        if rgmask[i]:
            lastfound = i
            break
    if lastfound is None:
        newnr = 0
    else:
        newnr = lastfound + 1
    if newnr < regs[0]:
        yield ('empty','','MT: compressed integer globals from %d to %d' % (regs[0], newnr))
        regs[0] = newnr

    # Compress FP locals
    if not fundata['hasjumps']:
        lastfound = None
        for i in xrange(30, -1, -1):
            if rfmask[i]:
                lastfound = i
                break
        if lastfound is None:
            newnr = 0
        else:
            newnr = lastfound + 1
        if newnr < regs[5]:
            yield ('empty','','MT: compressed FP locals from %d to %d' % (regs[5], newnr))
            regs[5] = newnr

    # Compress FP globals
    lastfound = None
    for i in xrange(30, -1, -1):
        if rgfmask[i]:
            lastfound = i
            break
    if lastfound is None:
        newnr = 0
    else:
        newnr = lastfound + 1
    if newnr < regs[3]:
        yield ('empty','','MT: compressed FP globals from %d to %d' % (regs[3], newnr))
        regs[3] = newnr

    for (type, content, comment) in items:
        yield (type, content, comment)

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
        hasload = fundata[keyname]
        regnr = fundata['specials'][regname]
        for (type, content, comment) in items:
            if content.startswith('.prologue') and hasload:
                yield ('other','clr $l%d' % regnr, 'init maybe used')
            yield (type, content, comment)
    return protectspecial

def protectallcallregs(fundata, items):
    if fundata['hasjumps']:
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
    if fundata['hasjumps']:
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

_filter_begin = [reader, lexer, splitsemi, parser, grouper]
_cfilter_inner = [crenameregs, stripmask]
_filter_inner = [
                regextract,
                renameregs,
                detectregs,
                replaceret,
                munchret,
                
                xsplit,
                
                avoidframespills,
                findcalls,
                protectcallsave,
                protectallcallregs,
                initspecial,
                #killgpreload,
                optprologue,
                
                #markused,
                #findusedcallregs,
                #protectcallregs,
                #rewritejsr,
                
                xjoin1,
                
                markused,
                makespecialtransform('gp'),
                makespecialtransform('tlsp'),
                makespecialtransform('fp'),
                makespecialtransform('ra'),
                makespecialtransform('pv'),
                
                compress,
                xjoin2,
                
                munchret,
                #makeprotectspecial('hasjumps','fp'),
                makedetectload('hasraload','ra'),
                makecompleteload('hasraload','ra'),
                makeprotectspecial('hasraload', 'ra'),
                makedetectload('hasfpload','fp'),
                makecompleteload('hasfpload','fp'),
                stripmask,
                ]
_filter_end = [flattener, forcezero, printer]

opts.register_arg('-e', action = "append", dest = "selection",
                  metavar = "STAGE", nargs = 1,
                  default = [],
                  help = 'Select ("STAGE") or unselect ("no-STAGE") the specified pass.')

def filter(*args):
    """ 
    Transform compiler output (invalid, raw asm) to good-looking
    .s files that can be assembled by an external assembler.
    """

    for e in [_filter_begin, _filter_inner, _cfilter_inner, _filter_end]:
        for s in e:
            opts.register_dump_stage(s.__name__)
    opts.parse_args(list(args))

    fname = opts.resolved.output
    if fname == "-":
        outf = sys.stdout
    else:
        try:
            outf = file(fname, "w")
        except Exception, e:
            die('%s: %r' % (fname, e))

    for inname in opts.inputs:
        items = inname
        for t in _filter_begin:
            items = t(items)
            items = dump.dump_gen(t.__name__, items)
        for t in _filter_inner:
            items = funfilter(t, items)
            items = dump.dump_gen(t.__name__, items)
        for t in _cfilter_inner:
            items = cfunfilter(t, items)
            items = dump.dump_gen(t.__name__, items)
        for t in _filter_end:
            items = t(items)
            items = dump.dump_gen(t.__name__, items)
        lines = items
        for line in lines:
            # print "YOO:", line,
            outf.write(line)
    outf.flush()
    if fname != "-": outf.close()

