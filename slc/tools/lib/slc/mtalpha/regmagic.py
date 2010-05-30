from ..msg import die, warn

# Magic numbers
_iargregs = 12
_fargregs = 12
_ilocalregs = 19
_flocalregs = 19

_legacy_fregs = {
   # Scratch registers
   'ft0' : 10,
   'ft1' : 11,
   'ft2' : 12,
   'ft3' : 13,
   'ft4' : 14,
   'ft5' : 15,
   'ft6' : 22,
   'ft7' : 23,
   'ft8' : 24,
   'ft9' : 25,
   'ft10' : 26,
   'ft11' : 27,
   'ft12' : 28,
   'ft13' : 29,
   'ft14' : 30,
   # Return value registers
   'frv' : 0,
   'frv2' : 1,
   # Incoming parameters
   'fa0' : 16,
   'fa1' : 17,
   'fa2' : 18,
   'fa3' : 19,
   'fa4' : 20,
   'fa5' : 21,
   # Call-save registers
   'fs0' : 2,
   'fs1' : 3,
   'fs2' : 4,
   'fs3' : 5,
   'fs4' : 6,
   'fs5' : 7,
   'fs6' : 8,
   'fs7' : 9,
   # Zero 
   'fzero' : 31,
   }

_legacy_regs = {
   # function temporaries
   't0' : 1,
   't1' : 2,
   't2' : 3,
   't3' : 4,
   't4' : 5,
   't5' : 6,
   't6' : 7,
   't7' : 8,
   't8' : 22,
   't9' : 23,
   't10' : 24,
   't11' : 25,
   # assembly scratch
   'at' : 28,
   # return value
   'rv' : 0,
   # incoming parameters
   'a5' : 21,
   'a4' : 20,
   'a3' : 19,
   'a2' : 18,
   'a1' : 17,
   'a0' : 16,
   # procedure function / temporary
   'pv' : 27,
   't12' : 27,
   # Callee-save registers
   's0' : 9,
   's1' : 10,
   's2' : 11,
   's3' : 12,
   's4' : 13,
   's5' : 14,
   # Return address
   'ra' : 26,
   # Frame pointer / callee save
   'fp' : 15,
   's6' : 15,
   # Global pointer
   'gp' : 29,
   # Stack pointer
   'sp' : 30,
   # Zero
   'zero' : 31
   }


######################
# GCC Allocation order
######################
# #define REG_ALLOC_ORDER {
#    1, 2, 3, 4, 5, 6, 7, 8,      /* nonsaved integer registers */
#    22, 23, 24, 25, 28,          /* likewise */
#    0,                           /* likewise, but return value */
#    21, 20, 19, 18, 17, 16,      /* likewise, but input args */
#    27,                          /* likewise, but OSF procedure value */
#    f10, f11, f12, f13, f14, f15,      /* nonsaved floating-point registers */
#    f22, f23, f24, f25, f26, f27,      /* likewise */
#    f28, f29, f30,                  /* likewise */
#    f0, f1,                      /* likewise, but return values */
#    f21, f20, f19, f18, f17, f16,      /* likewise, but input args */
#    9, 10, 11, 12, 13, 14,       /* saved integer registers */
#    26,                          /* return address */
#    15,                          /* hard frame pointer */
#    f2, f3, f4, f5, f6, f7,      /* saved floating-point registers */
#    f8, f9,                      /* likewise */
#    29, 30, 31, f31               /* gp, sp, ap, sfp */
# }


_reg_mapping = {
   # Globals
   'g0' : 't11',
   'g1' : 't10',
   'g2' : 't9',
   'g3' : 't8',
   'g4' : 't7',
   'g5' : 't6',
   'g6' : 't5',
   'g7' : 't4',
   'g8' : 't3',
   'g9' : 't2',
   'g10' : 't1',
   'g11' : 't0',

   'gf0' : 'ft11',
   'gf1' : 'ft10',
   'gf2' : 'ft9',
   'gf3' : 'ft8',
   'gf4'  : 'ft7',
   'gf5' : 'ft6',
   'gf6' : 'ft5',
   'gf7' : 'ft4',
   'gf8' : 'ft3',
   'gf9' : 'ft2',
   'gf10' : 'ft1',
   'gf11' : 'ft0',

   # Shareds
   's0' : 't0',
   'd0' : 't1',
   's1' : 't2',
   'd1' : 't3',
   's2' : 't4',
   'd2' : 't5',
   's3' : 't6',
   'd3' : 't7',
   's4' : 't8',
   'd4' : 't9',
   's5' : 't10',
   'd5' : 't11',

   'sf0' : 'ft0',
   'df0' : 'ft1',
   'sf1' : 'ft2',
   'df1' : 'ft3',
   'sf2' : 'ft4',
   'df2' : 'ft5',
   'sf3' : 'ft6',
   'df3' : 'ft7',
   'sf4' : 'ft8',
   'df4' : 'ft9',
   'sf5' : 'ft10',
   'df5' : 'ft11',

   'lf31' : 'fzero',

   'lf0' : 'ft12',
   'lf1' : 'ft13',
   'lf2' : 'ft14',
   'lf3' : 'frv',
   'lf4' : 'frv2',
   'lf5' : 'fa5',
   'lf6' : 'fa4',
   'lf7' : 'fa3',
   'lf8' : 'fa2',
   'lf9' : 'fa1',
   'lf10' : 'fa0',
   'lf11' : 'fs0',
   'lf12' : 'fs1',
   'lf13' : 'fs2',
   'lf14' : 'fs3',
   'lf15' : 'fs4',
   'lf16' : 'fs5',
   'lf17' : 'fs6',
   'lf18' : 'fs7',

   # Special locals
   'l31' : 'zero',
   'l18' : 'sp',
   'l17' : 'gp',
   'l16' : 'fp',
   'l15' : 'ra',
   'l14' : 'pv',
   'l13' : 's5',

   'l12' : 's4',
   'l11' : 's3',
   'l10' : 's2',
   'l9' : 's1',
   'l8' : 's0',
   'l7' : 'a0',
   'l6' : 'a1',
   'l5' : 'a2',
   'l4' : 'a3',
   'l3' : 'a4',
   'l2' : 'a5',
   'l1' : 'rv',
   'l0' : 'at',
   }

_reg_aliases = {
   'tlsp' : 'l18',
   'gp' : 'l17',
   'pv' : 'l14',
   'ra' : 'l15',
   'fp' : 'l16',
   'rv' : 'l1',
   'idx_init' : 'l0',
   'basearg' : 'l1',
   'zero' : 'l31',
   'frv' : 'lf3',
   'fzero' : 'lf31',
   'fbasearg' : 'lf1'
   }

_regs = { 
    'i' : { 'l' : [None]*32, 'g' : [None]*32, 's' : [None]*32, 'd' : [None]*32 },
    'f' : { 'l' : [None]*32, 'g' : [None]*32, 's' : [None]*32, 'd' : [None]*32 },
    }
_reg_inv = map(lambda x:[], xrange(32))
_freg_inv = map(lambda x:[], xrange(32))
_aliases = {}

### Module init ###    
for r,t in _reg_mapping.items():
    cat = r[0]
    if r[1] == 'f':
        species = 'f'
        nr = int(r[2:])
        legnr = _legacy_fregs[t]
        inv_dic = _freg_inv
    else:
        species = 'i'
        nr = int(r[1:])
        legnr = _legacy_regs[t]
        inv_dic = _reg_inv

    reg = { 'species' : species, 'cat' : cat, 'name' : r, 'legname' : t, 'nr' : nr, 'legnr' : legnr }
    if _aliases.has_key(r):
        die("mtalpha.init: alias %s already defined" % r)
    _aliases[r] = reg
    if _regs[species][cat][nr] is not None:
        die("mtalpha.init: register %s already defined" % r)
    _regs[species][cat][nr] = reg
    inv_dic[legnr].append(reg)

for a,k in _reg_aliases.items():
    if _aliases.has_key(a):
        die("mtalpha.init: alias %s already defined" % a)
    _aliases[a] = _aliases[k]

# Consistency check
for i in _reg_inv:
    if len(i) == 0:
        die("mtalpha.init: all legacy integer registers are not used")
if i in _freg_inv:
    if len(i) == 0:
        die("mtalpha.init: all legacy fp registers are not used")

### module services ###

def mapcall(args, funcname = '<unknown>', loc = None):
    """
    Compute a calling convention for the given actual argument
    list.
    """
    dic = { 
        'i' : { 'sh' : [], 'gl' : [] },
        'f' : { 'sh' : [], 'gl' : [] },
        }
    anames = {}
    #print args
    for a in args:
        assert a.type in ['sharg', 'shfarg',
                          'glarg', 'glfgarg',
                          'shparm','shfparm',
                          'glparm','glfparm',
                          'glparm_mutable', 'glfparm_mutable']
        cat = a.type[:2]
        if a.type[2] == 'f':
            species = 'f'                     
        else:
            species = 'i'
        if a.type.endswith('_mutable'):
            mode = 'w'
        else:
            mode = 'r'
            
        na = {'loc':getattr(a, 'loc', None),
              'cat':cat, 
              'species':species, 
              'ctype':a.ctype, 
              'name':a.name, 
              'init':getattr(a, 'init', None), 
              'glmode':mode}
        anames[a.name] = na
        dic[species][cat].append(na)

    # Check if some parameters will use memory passing
    escape = 0      
    if len(dic['f']['sh']) * 2 + len(dic['f']['gl']) > _fargregs:
        escape = 1
    if len(dic['i']['sh']) * 2 + len(dic['i']['gl']) > (_iargregs - escape):
        escape = 1

    # Start allocate
        
    islots_avail = _iargregs - escape
    fslots_avail = _fargregs

    # Allocate shareds first
    if len(dic['i']['sh']) * 2 > islots_avail:
        die("%s: too many int shareds, cannot pass them via memory yet" % funcname, 
            loc)
    shc = 0
    for s in dic['i']['sh']:
        s['mode'] = 'reg'
        s['regnr'] = shc
        shc += 1
        islots_avail -= 2
    nrishareds = shc

    if len(dic['f']['sh']) > fslots_avail:
        die("%s: too many float shareds, cannot pass them via memory yet" % funcname,
            loc)
    shc = 0
    for s in dic['f']['sh']:
        s['mode'] = 'reg'
        s['regnr'] = shc
        shc += 1
        fslots_avail -= 2
    nrfshareds = shc

    # Allocate fp globals
    glc = 0
    memlayout = []
    offset = 0
    for s in dic['f']['gl']:
        if fslots_avail > 0:
            s['mode'] = 'reg'
            s['regnr'] = glc
            glc += 1
            fslots_avail -= 1
        else:
            s['mode'] = 'mem'
            s['offset'] = offset
            offset += 1
            memlayout.append(s)
    nrfglobals = glc

    glc = 0
    for s in dic['i']['gl']:
        if islots_avail > 0:
            s['mode'] = 'reg'
            s['regnr'] = glc
            glc += 1
            islots_avail -= 1
        else:
            s['mode'] = 'mem'
            s['offset'] = offset
            offset += 1
            memlayout.append(s)
    nriglobals = glc

    ret = {}
    if escape == 0:
        ret['gl_mem_offset'] = None
    else:
        warn("%s: some arguments are passed via memory" % funcname, loc)
        nriglobals = glc + 1
        ret['gl_mem_offset'] = glc
        
    # nriglobals = nriglobals + 1 # count PV

    ret['nrargregs'] = { 'gli' : nriglobals, 'shi' : nrishareds,
                         'glf' : nrfglobals, 'shf' : nrfshareds }
    ret['args'] = dic
    ret['nargs'] = anames
    ret['memlayout'] = memlayout

    #pprint.pprint(ret)

    return ret

def get_legacy(legname):
    """
    Return the legacy canonical register name for
    a given legacy register alias.
    """
    if legname.startswith('f'):
        return '$f%d' % _legacy_fregs[legname]
    else:
        return '$%d' % _legacy_regs[legname]
    
def vreg_to_legacy(vreg):
    """
    Return the legacy canonical register name
    for a given virtual register.
    """
    if vreg['species'] == 'f':
        return "$f%d" % vreg['legnr']
    else:
        return "$%d" % vreg['legnr']

def get_vreg(vname):
    """
    Return the virtual register for a given
    (virtual) alias.
    """
    vname = vname.lstrip('$')
    return _aliases[vname]

def vname_to_legacy(vname):
    """
    Return the legacy canonical register name
    for a given virtual register alias.
    """
    return vreg_to_legacy(get_vreg(vname))

def get_vregs_for_legacy(legname):
    """
    Return the list of virtual registers that
    can overlap for the given legacy register alias.
    """
    if legname.startswith('$f'):
        return _freg_inv[int(legname[2:])]
    elif legname.startswith('f'):
        return _freg_inv[_legacy_fregs[legname]]
    elif legname.startswith('$'):
        return _reg_inv[int(legname[1:])]
    else:
        return _reg_inv[_legacy_regs[legname]]

def alias_to_vname(alias):
    """
    Return the canonical virtual register name
    for a given (virtual) alias.
    """
    return '$' + _reg_aliases[alias]

def makerepl(gli, shi, glf, shf):
    """ 
    Create a substitution function suitable for re.sub,
    based on the values given to .registers.
    """

    subst = {}
    regs = _regs
    for (spec, cat, nr, pref) in [('i','d',shi, ''),
                                  ('i','s',shi, ''),
                                  ('i','g',gli, ''),
                                  ('f','d',shf, 'f'),
                                  ('f','s',shf, 'f'),
                                  ('f','g',glf, 'f')]:
        for i in xrange(nr):
            r = regs[spec][cat][i]
            key = "%s%d" % (pref,r['legnr'])
            assert not subst.has_key(key)
            subst[key] = '$' + r['name']
    for (spec, pref) in [('i',''),('f','f')]:
        for r in (r for r in regs[spec]['l'] if r is not None):
            key = "%s%d" % (pref,r['legnr'])
            assert not subst.has_key(key)
            subst[key] = '$' + r['name']
    #print "MAKEREPL: ", subst
    def repl(match):
        r = match.group(1)
        #if r == "$31" or r == "$f31":
        #   return r
        return subst.get(r[1:], r)
    
    return repl


# all virtual register names
allregs = set(['$l%d' % x for x in xrange(31 - _iargregs) if x not in (17, 18)] + \
          ['$lf%d' % x for x in xrange(31 - _fargregs)] + \
          ['$d%d' % x for x in xrange(_iargregs / 2)] + \
          ['$df%d' % x for x in xrange(_fargregs / 2)] + \
          ['$%d' % x for x in xrange(31)])


def get_dot_registers(gli, shi, glf, shf):
    return ".registers %d %d %d %d %d %d" % (gli, shi, _ilocalregs,
                                             glf, shf, _flocalregs)


### Register lists for the compile commands ###
fixed_registers = []
for (v, l) in [("f",_freg_inv), ("",_reg_inv)]:
    fixed = set()
    for rl in l:
        for r in rl:
            if r['cat'] in 'sdg':
                fixed.add(r['legnr'])
    for r in fixed:
        fixed_registers.append('$%s%d' % (v, r))

call_used_registers = []
for i in xrange(6):
    call_used_registers.append('$%d' % _legacy_regs['s%d' % i])
for i in xrange(8):
    call_used_registers.append('$f%d' % _legacy_fregs['fs%d' % i])


