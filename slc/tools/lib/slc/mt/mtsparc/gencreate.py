from ...visitors import DefaultVisitor, ScopedVisitor, flatten
from ...ast import *
from regdefs import regmagic

class Create_2_MTSCreate(ScopedVisitor):
    
    def visit_seta(self, seta):
        b = seta.rhs.accept(self)

        name = seta.name

        adic = seta.lowcreate.callconv
        assert name in adic
        
        a = adic[name]

        if a['mode'] == 'mem':
            mavar = seta.lowcreate.mavar
            return (flatten(seta.loc, '(') +
                    mavar + ').%s = ' % name +
                    CVarSet(decl = seta.decl.cvar, rhs = b))
        else:
            return CVarSet(decl = seta.decl.cvar, rhs = b)

    def visit_lowcreate(self, lc):
        cr = self.cur_scope.creates[lc.label]

        # compute the calling convention
        c = regmagic.mapcall(cr.args, funcname = "create", loc = cr.loc)
        lc.callconv = c['nargs']

        newbl = Block(loc = cr.loc)
        lbl = cr.label

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
        
        # prepare structure for memory-passed arguments

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

        # Now handle body between create..sync

        newbl += lc.body.accept(self)

        # On to the real stuff. First try allocate

        fidvar = cr.cvar_fid
        
        usefvar = CVarUse(decl = fidvar)

        #if lc.target_next is None:
        #    if cr.extras.get_attr('exclusive', None) is None:
        #        warn("this create may fail and no alternative is available", cr)
        #    forcesuspend = '|8'
        #else:
        #    forcesuspend = ''

        newbl += (flatten(cr.loc,
                          '__asm__ __volatile__("allocate %%0\\t! MT: CREATE %s"'
                          ' : "=r"(' % lbl) + 
                  usefvar + '));')
        
        if lc.target_next is not None:
            newbl += (flatten(cr.loc, ' if (!__builtin_expect(!!(') + 
                      usefvar + '), 1)) ' + 
                      CGoto(target = lc.target_next)) + ';'
        
        start = CVarUse(decl = cr.cvar_start)
        limit = CVarUse(decl = cr.cvar_limit)
        step = CVarUse(decl = cr.cvar_step)
        block = CVarUse(decl = cr.cvar_block)
        newbl += (flatten(cr.loc, 
                         '__asm__ ("setstart %%0, %%2\\t! MT: CREATE %s"'
                         ' : "=r"(' % lbl) +
                  usefvar + ') : "0"(' + usefvar + '), "rP"(' + start + ')); ' +
                  '__asm__ ("setlimit %%0, %%2\\t! MT: CREATE %s"' % lbl +
                  ' : "=r"(' + usefvar + ') : "0"(' + usefvar  + '), "rP"(' + limit + ')); ' +
                  '__asm__ ("setstep %%0, %%2\\t! MT: CREATE %s"' % lbl +
                  ' : "=r"(' + usefvar + ') : "0"(' + usefvar + '), "rP"(' + step + ')); ' +
                  '__asm__ ("setblock %%0, %%2\\t! MT: CREATE %s"' % lbl +
                  ' : "=r"(' + usefvar + ') : "0"(' + usefvar + '), "rP"(' + block + ')); ' +
                  '__asm__ ("setthread %%0, %%2\\t! MT: CREATE %s"' % lbl +
                  ' : "=r"(' + usefvar + ') : "0"(' + usefvar + '), "rP"(' + funvar + ')); ')

        # FIXME: emit create+sync here
        # example:
        # {
        #     register X a1 reg("%l0") = arg1_val;
        #     ...
        #     asm("create ...; sync ..." : "=r"(a1) ... : "0"(a1) ...)
        #  }

        crc = Scope()
        aregn = 0
        gargs = []
        for g in c['gislots']:
            name = g['name']
            r = regmagic.vname_to_legacy("l%d" % aregn)
            var = CVarDecl(loc = cr.loc, 
                           name = 'C$aR$%s$%s' % (cr.label, name), 
                           ctype = g['ctype'],
                           init = CVarUse(decl = cr.arg_dic[name].cvar),
                           reg = r)
            crc.decls += var
            gargs.append(CVarUse(decl = var))
            aregn += 1

        if c['gl_mem_offset'] is not None:
            # one extra global var
            r = regmagic.vname_to_legacy("l%d" % aregn)
            var = CVarDecl(loc = cr.loc, 
                           name = 'C$aR$%s$%s' % (cr.label, name), 
                           ctype = mat + '*',
                           init = Opaque(text = '&') + mavar,
                           reg = r)
            crc.decls += var
            gargs.append(CVarUse(decl = var))
            aregn += 1

        collect = Block()
        sargs = []

        for s in c['sislots']:
            name = s['name']
            r = regmagic.vname_to_legacy("l%d" % aregn)
            arg_cvar = cr.arg_dic[name].cvar
            var = CVarDecl(loc = cr.loc, 
                           name = 'C$aR$%s$%s' % (cr.label, name), 
                           ctype = s['ctype'],
                           init = CVarUse(decl = arg_cvar),
                           reg = r)
            crc.decls += var
            sargs.append(CVarUse(decl = var))
            collect += CVarSet(decl = arg_cvar, rhs = CVarUse(decl = var)) + Opaque(';')
            aregn += 1

        # build reg arg lists
        # start with fidvar/shareds first, as these need to reference each other
        # and gcc has a limit on the number of back references
        olist = Opaque('"=r"(') + usefvar + ')'
        ilist = Opaque('"0"(') + usefvar + ')'
        roff = 1
        for v in sargs:
            olist +=  Opaque(', "=r"(') + v + ')'
            ilist +=  Opaque(', "%d"(' % roff) + v + ')'
            roff += 1
        for v in gargs:
            ilist +=  Opaque(', "r"(') + v + ')'

        crc += (flatten(cr.loc, 
                        '__asm__ __volatile__("create %%0, %%0\\t! MT: CREATE %s'
                        '\\n\\tmov %%0, %%0\\t! MT: SYNC %s" : ' % (lbl, lbl)) +
                olist + ' : ' + ilist + ' : "memory");')
        crc += collect

        newbl += crc

        return newbl


__all__ = ['Create_2_MTSCreate']
