from ..visitors import DefaultVisitor, ScopedVisitor, flatten
from ..ast import *
from ..msg import warn
import regmagic


class Create_2_MTACreate(ScopedVisitor):
    def __init__(self, *args, **kwargs):
        super(Create_2_MTACreate, self).__init__(*args, **kwargs)

    def visit_seta(self, seta):
        b = seta.rhs.accept(self)
        
        name = seta.name

        adic = seta.lowcreate.callconv
        fidvar = seta.lowcreate.fidvar

        # print "looking up %s in %r" % (name, self.__args)
        assert name in adic
        
        a = adic[name]
        
        newbl = []
        if a['mode'] == 'mem':
            mavar = seta.lowcreate.mavar
            newbl.append(flatten(seta.loc, '(') +
                         mavar + ').%s = ' % name +
                         CVarSet(decl = seta.decl.cvar, rhs = b))
        else:
            ctype = a['ctype']
            cat = a['cat']
            if cat == 'sh': 
                insn = 'puts'
                assign = b
            else: 
                assign = CVarSet(decl = seta.decl.cvar, rhs = b)
                insn = 'putg'
            if a['species'] == 'f': 
                insn = 'f' + insn
                rspec = 'f'
            else: rspec = 'rI'
            regnr = a['regnr']
            newbl.append(flatten(seta.loc, 
                                ' __asm__ ("%(insn)s %%2, %%0, %(regnr)d\\t# MT: set %(cat)sarg %(name)s"'
                                 ' : "=r"(' % locals()) +
                         fidvar + ') : "0"(' + fidvar + 
                         '), "%s"((' % rspec + ctype + ')(' +
                         assign + ')))')
        return newbl
                  

    def visit_lowcreate(self, lc):


        #print "IN LOWC (v = %x, d = %x, lc = %x)" % (id(self), id(self.__dict__), id(lc))
        cr = self.cur_scope.creates[lc.label]



        # compute the calling convention
        c = regmagic.mapcall(cr.args, funcname = "create", loc = cr.loc)

        newbl = Block(loc = cr.loc)
        lbl = cr.label

        # generate allocate + test for alternative
        fidvar = CVarDecl(loc = cr.loc, name = "C$mtF$%s" % lbl, ctype = 'long')
        self.cur_scope.decls += fidvar
        
        usefvar = CVarUse(decl = fidvar)

        if lc.target_next is None:
            if cr.extras.get_attr('exclusive', None) is None:
                warn("this create may fail and no alternative is available", cr)
            forcesuspend = '|8'
        else:
            forcesuspend = ''

        newbl += (flatten(cr.loc,
                          '__asm__ __volatile__("allocate %%1, %%0\\t# MT: CREATE %s"'
                          ' : "=r"(' % lbl) + 
                  usefvar + ') : "rI"(' + CVarUse(decl = cr.cvar_place) + 
                  '%s));' % forcesuspend)
        
        if lc.target_next is not None:
            newbl += (flatten(cr.loc, ' if (!__builtin_expect(!!(') + 
                      usefvar + '), 1)) ' + 
                      CGoto(target = lc.target_next)) + ';'


        # generate the function pointer
        if cr.funtype == cr.FUN_ID:
            if lc.lowfun is not None:
                funvar = lc.lowfun
            else:
                # not yet split
                funvar = Opaque(cr.fun)
        else:
            assert cr.funtype == cr.FUN_VAR

            n = 'C$mtF$%s' % lbl
            t = 'C$mtF$%s' % lbl
            
            thetype = CTypeDecl(loc = cr.loc,
                                name = t,
                                ctype = CType(items = 
                                              Opaque(text = "void (*") +
                                              CTypeHead() + 
                                              ')(void)'))
            self.cur_scope.decls += thetype
            funvar = CVarDecl(loc = cr.loc, name = n, ctype = CTypeUse(tdecl = thetype))
            self.cur_scope.decls += funvar

            if lc.lowfun is not None:
                thefun = lc.lowfun
            else:
                # not yet split
                thefun = CVarUse(decl = cr.fun)

            newbl += CVarSet(loc = cr.loc, decl = funvar, 
                             rhs = CCast(ctype = CTypeUse(tdecl = thetype),
                                         expr = thefun)) + ';'
            funvar = CVarUse(decl = funvar)

        # prepare memory structure for memory-passed arguments
            ### FIXME: move stuff to cur_scope
        if c['gl_mem_offset'] is not None:
            maname = "C$mtM$%s" % lbl
            mat = 'C$mtM$%s' % lbl

            thestruct = Opaque('struct {')
            for d in c['memlayout']:
                thestruct = thestruct + (Opaque(loc = d['loc']) + d['ctype'] + ' ' + d['name'] + ';')
            thestruct = thestruct + '}'

            thetype = CTypeDecl(loc = cr.loc,
                                name = mat,
                                ctype = thestruct)

            self.cur_scope.decls += thetype
            mavar = CVarDecl(loc = cr.loc, name = maname, ctype = CTypeUse(tdecl = thetype))
            self.cur_scope.decls += mavar
            mavar = CVarUse(decl = mavar)
            lc.mavar = mavar

        # generate create
        start = CVarUse(decl = cr.cvar_start)
        limit = CVarUse(decl = cr.cvar_limit)
        step = CVarUse(decl = cr.cvar_step)
        block = CVarUse(decl = cr.cvar_block)
        newbl += (flatten(cr.loc, 
                         '__asm__ ("setstart %%0, %%2\\t# MT: CREATE %s"'
                         ' : "=r"(' % lbl) +
                  usefvar + ') : "0"(' + usefvar + '), "rI"(' + start + ')); ' +
                  '__asm__ ("setlimit %%0, %%2\\t# MT: CREATE %s"' % lbl +
                  ' : "=r"(' + usefvar + ') : "0"(' + usefvar  + '), "rI"(' + limit + ')); ' +
                  '__asm__ ("setstep %%0, %%2\\t# MT: CREATE %s"' % lbl +
                  ' : "=r"(' + usefvar + ') : "0"(' + usefvar + '), "rI"(' + step + ')); ' +
                  '__asm__ ("setblock %%0, %%2\\t# MT: CREATE %s"' % lbl +
                  ' : "=r"(' + usefvar + ') : "0"(' + usefvar + '), "rI"(' + block + ')); ' +
                  '__asm__ __volatile__("wmb; crei %%0, 0(%%2)\\t# MT: CREATE %s"' % lbl +
                  ' : "=r"(' + usefvar + ') : "0"(' + usefvar + '),' +
                  '   "r"(' + funvar + ') : "memory");')

        lc.callconv = c['nargs']
        lc.fidvar = usefvar

        newbl += lc.body.accept(self)


        # done with body, now handle sync

        # first of all, if there weresome memory-passed arguments,
        # we need to push the argument register to the child family.
        
        if c['gl_mem_offset'] is not None:
            newbl += (flatten(cr.loc_end, 
                             ' __asm__ ("wmb; putg %%2, %%0, %d\\t#MT: set offset for memargs"' 
                             % c['gl_mem_offset']) + 
                      ' : "=r"(' + usefvar + ') : "0"(' + usefvar + '),' +
                      '   "r"(&' + mavar + '));')

        # now, on to the sync.
        if cr.sync_type == 'normal':
            # normal, synchronized create

            # first wait for child family to terminate.
            newbl += (flatten(cr.loc_end, 
                             '__asm__ __volatile__("sync %%0, %%1; '
                             ' mov %%1, $31\\t# MT: SYNC %s"' % lbl) +
                      ' : "=r"(' + usefvar + '), "=r"(' + 
                      CVarUse(decl = cr.cvar_exitcode) + 
                      ') : "0"(' + usefvar + ') : "memory");')
            
            # then pull shared arguments back.
            for name, arg in c['nargs'].iteritems():
                crarg = cr.arg_dic[name]
                if not crarg.seen_get:
                    # geta() is not used, so no need to retrieve
                    continue
                if arg['mode'] == 'reg' and arg['cat'] == 'sh':
                    if arg['species'] == 'f':
                        insn1 = 'fgets'
                        insn2 = 'fmov'
                        rspec = 'f'
                    else:
                        insn1 = 'gets'
                        insn2 = 'mov'
                        rspec = 'r'
                    regnr = arg['regnr']
                    argvar = crarg.cvar
                    # FIXME: perform "mov" after all "get" have been issued!
                    newbl += (flatten(cr.loc_end, 
                                     ' __asm__ ('
                                     '"%(insn1)s %%0, %(regnr)d, %%1; '
                                     ' %(insn2)s %%1, %%1'
                                     '\\t# MT: get shared %(name)s"' % locals()) +
                              ' : "=r"(' + usefvar + '), "=%(rspec)s"(' % locals() +
                              CVarUse(decl = argvar) + ') : "0"(' + usefvar + '));')
    
                          
        # in call cases (sync and detach), release resources.
        newbl += (flatten(cr.loc_end, 
                         ' __asm__ __volatile__("release %%0\\t#MT: SYNC %s"' % lbl) +
                  ' : : "r"(' + usefvar + '));')

        return newbl



class TFun_2_MTATFun(DefaultVisitor):
    
    def visit_fundecl(self, fundecl):
        return flatten(fundecl.loc, " void %s(void)" % fundecl.name)

    def visit_fundeclptr(self, fundecl):
        if fundecl.extras.get_attr('typedef', None) is not None:
            qual = 'typedef'
        elif fundecl.extras.get_attr('static', None) is not None:
            qual = 'static'
        else:
            qual = ''
        
        return flatten(fundecl.loc, " %s void (*%s)(void)" % (qual, fundecl.name))
    
    def visit_fundef(self, fundef):
        newitems = []
        
        # first compute the call convention
        c = regmagic.mapcall(fundef.parms, 
                             funcname = "function '%s'" % fundef.name, 
                             loc = fundef.loc)
        
        
        # generate the function header
        regdir = regmagic.get_dot_registers(c['nrargregs']['gli'],
                                            c['nrargregs']['shi'],
                                            c['nrargregs']['glf'],
                                            c['nrargregs']['shf'])
        idxreg = regmagic.vname_to_legacy("idx_init")

        newitems.append(flatten(fundef.loc,
                                ' extern void %(name)s(void); '
                                'void __slf_%(name)s(void) {'
                                ' register long __slI_ __asm__("%(idxreg)s");'
                                ' __asm__("# MT: index in %%0 (must be %(idxreg)s)"'
                                '   : "=r"(__slI_));'
                                ' register const long __slI = __slI_;'
                                ' __asm__ __volatile__("%(regdir)s");'
                                % { 'name': fundef.name, 
                                    'regdir' : regdir,
                                    'idxreg' : idxreg }))
        
        # analyze the parameter list
        self.gllist = []
        self.shlist = []
        self.gllist_mem = []
        self.gllist_mutable = []
        

        if c['gl_mem_offset'] is not None:
            # some parameters are passed using memory.
            newitems.append(flatten(fundef.loc, "register const struct {"))
            
            for d in c['memlayout']:
                # keep them for getp/setp
                self.gllist_mem.append(d['name'])
                
                # also issue a field in the data structure
                newitems.append(flatten(d['loc'], ' ') + d['ctype'] + ' %s' % d['name'] + ';')
                
            memescape_reg = regmagic.vname_to_legacy('g%d' % c['gl_mem_offset'])
            newitems.append(flatten(fundef.loc, 
                                    '} * __restrict__ const __slPgm __asm__("%s");' 
                                    % memescape_reg))

        for species, d1 in c['args'].items():
            for cat, d2 in d1.items():
                for a in d2:
                    name = a['name']
                    ctype = a['ctype']
                    if a['mode'] == 'mem':
                        assert cat == 'gl'
                        self.gllist_mem.append(name)
                    else: # mode: reg
                        regnr = a['regnr']
                        if cat == "sh":
                            self.shlist.append(name)                              
                            if species == "f":                                   
                                dreg = "df%d" % regnr
                                sreg = "sf%d" % regnr
                            else:
                                dreg = "d%d" % regnr
                                sreg = "s%d" % regnr
                            dreg = regmagic.vname_to_legacy(dreg)
                            sreg = regmagic.vname_to_legacy(sreg)
                            newitems.append(flatten(a['loc'], 'register ') +
                                            ctype + ' __slPsin_%(name)s __asm__("%(dreg)s"); '
                                            'register ' % locals() + ctype + 
                                            ' __slPsout_%(name)s __asm__("%(sreg)s"); '  % locals())

                        else: # cat: gl
                            self.gllist.append(name)                              
                            if species == "f":
                                reg = "gf%d" % regnr
                            else:
                                reg = "g%d" % regnr
                            reg = regmagic.vname_to_legacy(reg)
                            newitems.append(flatten(a['loc'], 'register ') + 
                                            ctype + 
                                            ' const __slPg_%(name)s __asm__("%(reg)s"); ' % locals())

        # For every global parameter which is also declared
        # mutable, we have to create a "regular" variable for it
        # and trace it in gllist_mutable for getp.
        for p in fundef.parms:
            if p.type.endswith('_mutable'):
                self.gllist_mutable.append(p.name)
                if p.name in self.gllist_mem:
                    orig = "__slPgm->%s" % p.name
                else:
                    orig = "__slPg_%s" % p.name
                newitems.append(flatten(p.loc,'') + p.ctype + 
                                " __slPwg_%s = %s;" % (p.name, orig))

        # consume the body
        fundef.lbl_end = CLabel(loc = fundef.loc_end, name = "mtend")
        self.cur_fun = fundef
        newitems.append(fundef.body.accept(self))

        # close the body definition with a target label for sl_end_thread
        newitems.append(CGoto(loc = fundef.loc_end, target = fundef.lbl_end) + ';' 
                        + fundef.lbl_end 
                        + ' __asm__ __volatile__("# MT: end follows"); }')
        return newitems

    def visit_getp(self, getp):
        if getp.name in self.gllist_mutable:
            return flatten(getp.loc, ' __slPwg_%s ' % getp.name)
        elif getp.name in self.shlist:
            name = getp.name
            return flatten(getp.loc, 
                           '({'
                           '__asm__ __volatile__("# MT: read shared %(name)s (%%0)"'
                           '  : "=rf"(__slPsin_%(name)s)'
                           '  : "0"(__slPsin_%(name)s));'
                           '__slPsin_%(name)s;'
                           '})'
                           % locals())
        elif getp.name in self.gllist_mem:
            return flatten(getp.loc, "(__slPgm->%s)" % getp.name)
        else: # normal global
            assert getp.name in self.gllist
            return flatten(getp.loc, " __slPg_%s " % getp.name)

    def visit_setp(self, setp):
        rhs = setp.rhs.accept(self)
        if setp.name in self.shlist:
            name = setp.name
            b = []
            b.append(flatten(setp.loc, 
                             'do {'
                             ' __asm__ __volatile__("# MT: clobber incoming %(name)s (%%0)"'
                             ' : "=rf"(__slPsin_%(name)s) '
                             ' : "0"(__slPsin_%(name)s));'
                             ' __typeof__(__slPsout_%(name)s) __tmp_set_%(name)s = ('
                             % locals()))
            b.append(rhs)
            b.append(flatten(setp.loc, 
                             ');'
                             ' __asm__ __volatile__("# MT: start write shared %(name)s (%%0)"'
                             ' : "=rf"(__slPsout_%(name)s) : "0"(__slPsout_%(name)s));'
                             ' __asm__ __volatile__("mov %%4, %%0\\t# MT: write shared %(name)s (%%0)"'
                             ' : "=rf" (__slPsout_%(name)s), "=rf" (__slPsin_%(name)s)'
                             ' : "0"(__slPsout_%(name)s), "1" (__slPsin_%(name)s), '
                             '   "rf" (__tmp_set_%(name)s));'
                             '} while(0)'
                             % locals()))
            return b
        else:
            assert setp.name in self.gllist_mem or setp.name in self.gllist
            return flatten(setp.loc, "#error invalid set to global %s\n" % setp.name)

    def visit_break(self, br):
        return flatten(br.loc, 
                       ' do {'
                       ' __asm__ __volatile__("break; end\\t# MT: break");'
                       ' __builtin_unreachable();'
                       '} while(0)')
    
    def visit_endthread(self, et):
        return CGoto(loc = et.loc, target = self.cur_fun.lbl_end)

    def visit_indexdecl(self, idecl):
        return flatten(idecl.loc, 
                       " register const long %s = __slI " 
                       % idecl.indexname) 

__all__ = ['Create_2_MTACreate', 'TFun_2_MTATFun']
