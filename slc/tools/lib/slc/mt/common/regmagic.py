from ...msg import die, warn

class RegMagic:


    def __init__(self, rd):

        self._regs = { 
        'i' : { 'l' : [None]*32, 'g' : [None]*32, 's' : [None]*32, 'd' : [None]*32 },
        'f' : { 'l' : [None]*32, 'g' : [None]*32, 's' : [None]*32, 'd' : [None]*32 },
        }
        self._reg_inv = map(lambda x:[], xrange(32))
        self._freg_inv = map(lambda x:[], xrange(32))
        self._aliases = {}
        
        self.rd = rd
        self.comprefix = rd.comprefix
        aliases = self._aliases

        for r,t in rd.reg_mapping.items():
            cat = r[0]
            if r[1] == 'f':
                species = 'f'
                nr = int(r[2:])
                legnr = rd.legacy_fregs[t]
                inv_dic = self._freg_inv
            else:
                species = 'i'
                nr = int(r[1:])
                legnr = rd.legacy_regs[t]
                inv_dic = self._reg_inv

            reg = { 'species' : species, 'cat' : cat, 'name' : r, 'legname' : t, 'nr' : nr, 'legnr' : legnr }
            if aliases.has_key(r):
                die("mt.regmagic: alias %s already defined" % r)
            aliases[r] = reg
            if self._regs[species][cat][nr] is not None:
                die("mt.regmagic: register %s already defined" % r)
            self._regs[species][cat][nr] = reg
            inv_dic[legnr].append(reg)

        for a,k in rd.reg_aliases.items():
            if aliases.has_key(a):
                die("mt.regmagic: alias %s already defined" % a)
            aliases[a] = aliases[k]

        # Consistency check
        for i in self._reg_inv:
            if len(i) == 0:
                die("mt.regmagic: all legacy integer registers are not used")
        if i in self._freg_inv:
            if len(i) == 0:
                die("mt.regmagic: all legacy fp registers are not used")


        # machine-specific post-initialisation
        rd.post_init_regmagic(self)

    ### services ###

    def mapcall(self,args, funcname = '<unknown>', loc = None):
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
            if a.type not in ['sharg', 'shfarg',
                              'glarg', 'glfarg',
                              'shparm','shfparm',
                              'glparm','glfparm',
                              'glparm_mutable', 'glfparm_mutable']:
                die("unknown thread parameter/argument type: %s" % a.type, loc)
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
        if len(dic['f']['sh']) * 2 + len(dic['f']['gl']) > self.rd.fargregs:
            escape = 1
        if len(dic['i']['sh']) * 2 + len(dic['i']['gl']) > (self.rd.iargregs - escape):
            escape = 1

        # Start allocate

        islots_avail = self.rd.iargregs - escape
        fslots_avail = self.rd.fargregs
        sislots = []
        sfslots = []
        gislots = []
        gfslots = []

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
            sislots.append(s)
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
            sfslots.append(s)
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
                gfslots.append(s)
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
                gislots.append(s)
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
        ret['sislots'] = sislots
        ret['sfslots'] = sfslots
        ret['gislots'] = gislots
        ret['gfslots'] = gfslots

        #pprint.pprint(ret)

        return ret

    def makecom(self, com):
        return '%s %s' % (self.rd.comprefix, com)

    def makecanon(self, spec, nr, legname_hint = None):
        if self.rd.canon_is_numeric:
            p = ''
            if spec == 'f': p = 'f'
            return '%s%s%d' % (self.rd.regprefix, p, nr)
        else:
            if legname_hint is not None:
                return legname_hint
            if spec == 'f':
                d = self._freg_inv
            else:
                d = self._reg_inv
            return '%s%s' % (self.rd.regprefix, d[nr][0]['legname'])

    def get_legacy(self, legname):
        """
        Return the legacy canonical register name for
        a given legacy register alias.
        """
        if legname.startswith('f'):
            return self.makecanon('f', self.rd.legacy_fregs[legname], legname_hint = legname)
        else:
            return self.makecanon('i', self.rd.legacy_regs[legname], legname_hint = legname)

    def vreg_to_legacy(self, vreg):
        """
        Return the legacy canonical register name
        for a given virtual register.
        """
        return self.makecanon(vreg['species'], vreg['legnr'])
 
    def get_vreg(self,vname):
        """
        Return the virtual register for a given
        (virtual) alias.
        """
        vname = vname.lstrip(self.rd.regprefix)
        return self._aliases[vname]

    def vname_to_legacy(self,vname):
        """
        Return the legacy canonical register name
        for a given virtual register alias.
        """
        return self.vreg_to_legacy(self.get_vreg(vname))

    def get_vregs_for_legacy(self,legname):
        """
        Return the list of virtual registers that
        can overlap for the given legacy register alias.
        """
        if legname.startswith('%sf' % self.rd.regprefix):
            return self._freg_inv[self.rd.legacy_fregs[legname[1:]]]
        elif legname.startswith('f'):
            return self._freg_inv[self.rd.legacy_fregs[legname]]
        elif legname.startswith(self.rd.regprefix):
            return self._reg_inv[self.rd.legacy_regs[legname[1:]]]
        else:
            return self._reg_inv[self.rd.legacy_regs[legname]]

    def alias_to_vname(self,alias):
        """
        Return the canonical virtual register name
        for a given (virtual) alias.
        """
        return self.rd.regprefix + self.rd.reg_aliases[alias]

    def makecrepl(self,funname):
        """
        Create a substitution function suitable for re.sub,
        for renaming C-used registers to the base of the register
        window.
        """

        subst = {}
        regs = self._regs
        for (spec, pref) in [('i',''),('f','f')]:
            for r in (r for r in regs[spec]['l'] if r is not None):
                key = "%s%s%d" % (self.rd.regprefix, pref,r['legnr'])
                assert not subst.has_key(key)
                subst[key] = self.rd.regprefix + r['name'][1:]
        import sys
        #print >>sys.stderr, "XXX", subst
        def repl(match):
            k = match.group(1)
            if k not in subst:
                raise die("in function %s: unsupported use of register %s" % (funname, k))
            return subst[k]
        return repl

    def makerepl(self, gli, shi, glf, shf):
        """ 
        Create a substitution function suitable for re.sub,
        based on the values given to .registers.
        """

        subst = {}
        regs = self._regs
        pr = self.rd.regprefix
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
                subst[key] = pr + r['name']
        for (spec, pref) in [('i',''),('f','f')]:
            for r in (r for r in regs[spec]['l'] if r is not None):
                key = "%s%d" % (pref,r['legnr'])
                assert not subst.has_key(key)
                subst[key] = pr + r['name']
        #print "MAKEREPL: ", subst
        def repl(match):
            r = match.group(1)
            #if r == pr+"31" or r == pr+"f31":
            #   return r
            return subst.get(r[1:], r)

        return repl


    def get_dot_registers(self, gli, shi, glf, shf):
        return ".registers %d %d %d %d %d %d" % (gli, shi, self.rd.ilocalregs,
                                                 glf, shf, self.rd.flocalregs)



