from ...visitors import DefaultVisitor, flatten
from ...ast import *

class TFun_2_MTFun(DefaultVisitor):

    def __init__(self, regmagic, *args, **kwargs):
        self.regmagic = regmagic
        super(TFun_2_MTFun, self).__init__(*args, **kwargs)

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
        regmagic = self.regmagic
        cp = regmagic.comprefix

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
                                ' __asm__("%(cp)s MT: index in %%0"'
                                '   : "=r"(__slI_));'
                                ' register const long __slI __attribute__((unused)) = __slI_;'
                                ' __asm__ __volatile__("%(regdir)s");'
                                % { 'cp' : cp, 
                                    'name': fundef.name, 
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
                                regtype = "f"
                            else:
                                reg = "g%d" % regnr
                                regtype = "r"
                            reg = regmagic.vname_to_legacy(reg)
                            igname = '__slPgi_%s' % name
                            gname = '__slPg_%s' % name
                            newitems.append(flatten(a['loc'], 'register ') + 
                                            ctype + 
                                            ' %(igname)s __asm__("%(reg)s"); ' 
                                            ' __asm__ __volatile__("%(cp)s MT: global init %%0"'
                                            ' : "=%(regtype)s"(%(igname)s)); '
                                            ' register ' % locals() + 
                                            ctype + ' const %(gname)s = %(igname)s; ' % locals())

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
                        + ' __asm__ __volatile__("%s MT: end follows"); }' % cp)
        return newitems

    def visit_getp(self, getp):
        name = getp.name
        if name in self.shlist:
            if getp.decl.type == 'shfparm':
                regtype = 'f'
            else:
                regtype = 'r'
            cp = self.regmagic.comprefix
            return flatten(getp.loc, 
                           '({'
                           '__asm__ __volatile__("%(cp)s MT: read shared %(name)s (%%0)"'
                           '  : "=%(regtype)s"(__slPsin_%(name)s)'
                           '  : "0"(__slPsin_%(name)s));'
                           '__slPsin_%(name)s;'
                           '})'
                           % locals())
        elif name in self.gllist_mutable:
            return flatten(getp.loc, ' __slPwg_%s ' % name)
        elif name in self.gllist_mem:
            return flatten(getp.loc, "(__slPgm->%s)" % name)
        else: # normal global
            assert name in self.gllist
            return flatten(getp.loc, " __slPg_%s " % name)

    def visit_setp(self, setp):
        rhs = setp.rhs.accept(self)
        name = setp.name
        if name in self.shlist:
            if setp.decl.type == 'shfparm':
                regtype = 'f'
                movinsn = self.regmagic.rd.fmovinsn
            else:
                regtype = 'r'
                movinsn = self.regmagic.rd.movinsn
            b = []
            cp = self.regmagic.comprefix
            b.append(flatten(setp.loc, 
                             'do {'
                             ' __asm__ __volatile__("%(cp)s MT: clobber incoming %(name)s (%%0)"'
                             ' : "=%(regtype)s"(__slPsin_%(name)s) '
                             ' : "0"(__slPsin_%(name)s));'
                             ' __typeof__(__slPsout_%(name)s) __tmp_set_%(name)s = ('
                             % locals()))
            b.append(rhs)
            b.append(flatten(setp.loc, 
                             ');'
                             ' __asm__ __volatile__("%(cp)s MT: start write shared %(name)s (%%0)"'
                             ' : "=%(regtype)s"(__slPsout_%(name)s) : "0"(__slPsout_%(name)s));'
                             ' __asm__ __volatile__("%(movinsn)s %%4, %%0\\t%(cp)s MT: write shared %(name)s (%%0)"'
                             ' : "=%(regtype)s" (__slPsout_%(name)s), "=%(regtype)s" (__slPsin_%(name)s)'
                             ' : "0"(__slPsout_%(name)s), "1" (__slPsin_%(name)s), '
                             '   "%(regtype)s" (__tmp_set_%(name)s));'
                             '} while(0)'
                             % locals()))
            return b
        else:
            assert name in self.gllist_mem or name in self.gllist
            return flatten(setp.loc, "#error invalid set to global %s\n" % name)

    def visit_break(self, br):
        return flatten(br.loc, 
                       ' do {'
                       ' __asm__ __volatile__("break; end\\t%s MT: break");'
                       ' __builtin_unreachable();'
                       ' while(1);'
                       '} while(0)' % self.regmagic.comprefix)
    
    def visit_endthread(self, et):
        return CGoto(loc = et.loc, target = self.cur_fun.lbl_end)

    def visit_indexdecl(self, idecl):
        return flatten(idecl.loc, 
                       " register const long %s = __slI " 
                       % idecl.indexname) 
