from ..visitors import DefaultVisitor, flatten
from ..ast import LowCreate, Block
from ..msg import warn
import regmagic


class Create_2_MTACreate(DefaultVisitor):
    def __init__(self, *args, **kwargs):
        super(Create_2_MTACreate, self).__init__(*args, **kwargs)
        self.__args = None
        self.__maname = None
        self.__fidvar = None
        
    def visit_geta(self, geta):
        return flatten(geta.loc, LowCreate.arg_var(geta.name))

    def visit_seta(self, seta):
        b = seta.rhs.accept(self)
        
        name = seta.name
        # print "looking up %s in %r" % (name, self.__args)
        assert self.__args.has_key(name)
        
        a = self.__args[name]
        fidvar = self.__fidvar
        
        newbl = []
        if a['mode'] == 'mem':
            maname = self.__maname
            newbl.append(flatten(seta.loc, 
                                 " %s.%s = %s = " 
                                 % (maname, name, LowCreate.arg_var(name))))
            newbl.append(b)
        else:
            ctype = a['ctype']
            cat = a['cat']
            if cat == 'sh': 
                insn = 'puts'
                assign = ""
            else: 
                assign = " %s = " % LowCreate.arg_var(name)
                insn = 'putg'
            if a['species'] == 'f': 
                insn = 'f' + insn
                rspec = 'f'
            else: rspec = 'rI'
            regnr = a['regnr']
            newbl.append(flatten(seta.loc, 
                                 ' __asm__ ("%(insn)s %%2, %%0, %(regnr)d\\t# MT: set %(cat)sarg %(name)s"'
                                 ' : "=r"(%(fidvar)s)'
                                 ' : "0"(%(fidvar)s),'
                                 ' "%(rspec)s"((%(ctype)s)(%(assign)s('
                                 % locals()))
            newbl.append(b)
            newbl.append(flatten(seta.loc, "))))"))
        return newbl
                  

    def visit_lowcreate(self, cr):

        # compute the calling convention
        c = regmagic.mapcall(cr.args, funcname = "create", loc = cr.loc)

        newbl = Block(loc = cr.loc)
        lbl = cr.label

        # generate allocate + test for alternative
        fidvar = "__slC_mtfid_%s" % lbl
        newbl += flatten(cr.loc, 
                         ' long %(fidvar)s;'
                         '__asm__ __volatile__("allocate %%1, %%0\\t# MT: CREATE %(lbl)s"'
                         ' : "=r"(%(fidvar)s) : "rI"(%(place)s)); '
                         % { 'lbl': lbl, 'fidvar' : fidvar, 'place' : cr.place })
        
        if cr.target_next is None:
            # FIXME: ignore warning if the create is guaranteed to wait
            warn("this create may fail and no alternative is available", cr)
        else:
            newbl += flatten(cr.loc,
                             ' if (!__builtin_expect(!!(%s), 1)) goto %s ;' % (fidvar, cr.target_next))


        # generate the function pointer
        if cr.funtype == cr.FUN_ID:
            funvar = cr.fun
        else:
            funvar = '__slC_mtfun_%s' % lbl
            newbl += flatten(cr.loc, 
                             " void (*%s)(void) ="
                             "(void (*)(void))(%s);" % (funvar, cr.fun))

        # prepare memory structure for memory-passed arguments
        maname = "__slC_ma_%s" % lbl
        if c['gl_mem_offset'] is not None:
            newbl += flatten(cr.loc, " struct {")
            for d in c['memlayout']:
                newbl += flatten(d['loc'], "%s %s;" % (d['ctype'], d['name']))
            newbl += flatten(cr.loc, "} %s;" % maname)

        # generate create
        newbl += flatten(cr.loc, 
                         '__asm__ ("setstart %%0, %%2\\t# MT: CREATE %(lbl)s"'
                         ' : "=r"(%(fidvar)s) : "0"(%(fidvar)s), "rI"(%(start)s)); '
                         '__asm__ ("setlimit %%0, %%2\\t# MT: CREATE %(lbl)s"'
                         ' : "=r"(%(fidvar)s) : "0"(%(fidvar)s), "rI"(%(limit)s)); '
                         '__asm__ ("setstep %%0, %%2\\t# MT: CREATE %(lbl)s"'
                         ' : "=r"(%(fidvar)s) : "0"(%(fidvar)s), "rI"(%(step)s));'
                         '__asm__ ("setblock %%0, %%2\\t# MT: CREATE %(lbl)s"'
                         ' : "=r"(%(fidvar)s) : "0"(%(fidvar)s), "rI"(%(block)s));'
                         '__asm__ __volatile__("wmb; crei %%0, 0(%%2)\\t# MT: CREATE %(lbl)s"'
                         ' : "=r"(%(fidvar)s) : "0"(%(fidvar)s),'
                         '   "r"(%(funvar)s) : "memory");'
                         % { 'lbl' : lbl,
                             'funvar' : funvar,
                             'fidvar' : fidvar,
                             'start' : cr.start,
                             'limit' : cr.limit,
                             'step' : cr.step,
                             'block' : cr.block })

        old_maname = self.__maname
        old_args = self.__args 
        old_fidvar = self.__fidvar
        self.__args = c['nargs']
        self.__fidvar = fidvar
        self.__maname = maname

        newbl += cr.body.accept(self)

        self.__args = old_args
        self.__fidvar = old_fidvar
        self.__maname = old_maname

        # done with body, now handle sync

        # first of all, if there weresome memory-passed arguments,
        # we need to push the argument register to the child family.
        
        if c['gl_mem_offset'] is not None:
              newbl += flatten(cr.loc_end, 
                               ' __asm__ ("wmb; putg %%2, %%0, %(offset)d\\t#MT: set offset for memargs"'
                               ' : "=r"(%(fidvar)s) : "0"(%(fidvar)s),'
                               '   "r"(&__slC_ma_%(lbl)s));'
                               % { 'lbl' : lbl, 
                                   'fidvar' : fidvar,
                                   'offset' : c['gl_mem_offset']} )

        # now, on to the sync.
        if cr.sync_type == 'normal':
            # normal, synchronized create

            # first wait for child family to terminate.
            newbl += flatten(cr.loc_end, 
                             '__asm__ __volatile__("sync %%0, %%1; '
                             ' mov %%1, $31\\t# MT: SYNC %(lbl)s"'
                             ' : "=r"(%(fidvar)s), "=r"(%(retvar)s)'
                             ' : "0"(%(fidvar)s) : "memory");'
                             % { 'lbl' : lbl, 
                                 'fidvar' : fidvar,
                                 'retvar' : cr.retval })
            
            # then pull shared arguments back.
            for name, arg in c['nargs'].iteritems():
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
                    argvar = cr.arg_var(name)
                    # FIXME: perform "mov" after all "get" have been issued!
                    newbl += flatten(cr.loc_end, 
                                     ' __asm__ ('
                                     '"%(insn1)s %%0, %(regnr)d, %%1; '
                                     ' %(insn2)s %%1, %%1'
                                     '\\t# MT: get shared %(name)s"'
                                     ' : "=r"(%(fidvar)s), "=%(rspec)s"(%(argvar)s)'
                                     ' : "0"(%(fidvar)s));'
                                     % locals())
    
                          
        # in call cases (sync and detach), release resources.
        newbl += flatten(cr.loc_end, 
                         ' __asm__ __volatile__("release %%0\\t#MT: SYNC %(lbl)s"'
                         ' : : "r"(%(fidvar)s));'
                         % locals())
        return newbl



class TFun_2_MTATFun(DefaultVisitor):
    
    def visit_fundecl(self, fundecl):
        return flatten(fundecl.loc, "void %s(void)" % fundecl.name)
    
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
                                'extern void %(name)s(void); '
                                'void __slf_%(name)s(void) {'
                                ' register long __slI __asm__("%(idxreg)s");'
                                ' __asm__("# MT: index in %%0 (must be %(idxreg)s)"'
                                '   : "=r"(__slI));'
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
                newitems.append(flatten(d['loc'], "%s %s;" % (d['ctype'], d['name'])))
                
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
                            newitems.append(flatten(a['loc'], 
                                                    'register %(ctype)s __slPsin_%(name)s __asm__("%(dreg)s"); '
                                                    'register %(ctype)s __slPsout_%(name)s __asm__("%(sreg)s"); '
                                                    % locals()))

                        else: # cat: gl
                            self.gllist.append(name)                              
                            if species == "f":
                                reg = "gf%d" % regnr
                            else:
                                reg = "g%d" % regnr
                            reg = regmagic.vname_to_legacy(reg)
                            newitems.append(flatten(a['loc'], 
                                                    'register %(ctype)s const __slPg_%(name)s __asm__("%(reg)s"); ' 
                                                    % locals()))

        # For every global parameter which is also declared
        # mutable, we have to create a "regular" variable for it
        # and trace it in gllist_mutable for getp.
        for p in fundef.parms:
            if p.type.endswith('_mutable'):
                self.gllist_mutable.append(p.name)
                if p.name in self.gllist_mem:
                    orig = "__slPgm->%s" % p.name
                else:
                    orig = "__slPg_%s" % p.aname
                newitems.append(flatten(p.loc, 
                                        "%s __slPwg_%s = %s;" 
                                        % (p.ctype, p.name, orig)))

        # consume the body
        newitems.append(fundef.body.accept(self))

        # close the body definition with a target label for sl_end_thread
        newitems.append(flatten(fundef.loc_end, "__sl_end: (void)0; }"))
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
        return flatten(et.loc, " goto __sl_end ")

    def visit_indexdecl(self, idecl):
        return flatten(idecl.loc, 
                       " register const long %s = __slI " 
                       % idecl.indexname) 

__all__ = ['Create_2_MTACreate', 'TFun_2_MTATFun']
