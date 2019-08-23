from ...visitors import DefaultVisitor, ScopedVisitor, flatten
from ...ast import *
from ...msg import warn,die
from regdefs import regmagic

class Create_2_MTSCreate(ScopedVisitor):

    def __init__(self, newisa = False, *args, **kwargs):
        self.newisa = newisa
        super(Create_2_MTSCreate, self).__init__(*args, **kwargs)
    
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

            matype = CTypeDecl(loc = cr.loc,
                                name = mat,
                                ctype = thestruct)

            self.cur_scope.decls += matype
            mavar = CVarDecl(loc = cr.loc, name = maname, ctype = CTypeUse(tdecl = matype))
            self.cur_scope.decls += mavar
            mavar = CVarUse(decl = mavar)
            lc.mavar = mavar

        fidvar = cr.cvar_fid
        usefvar = CVarUse(decl = fidvar)
        lc.fidvar = usefvar

        # Prepare the family.

        if cr.extras.has_attr('exclusive'):
            if not self.newisa:
                die("exclusive create not supported on this target", cr)
            allocinsn = 'allocatex'
        elif lc.target_next is None:
            if cr.extras.has_attr('nowait'):
                warn("this create may fail and no alternative is available", cr)
                allocinsn = 'allocate'
            else:
                allocinsn = 'allocates'
        else:
            if cr.extras.has_attr('forcewait'):
                allocinsn = 'allocates'
            else:
                allocinsn = 'allocate'
        
        if allocinsn == 'allocates' and not self.newisa:
                die("suspending create is not supported on this target", cr)

        start = CVarUse(decl = cr.cvar_start)
        limit = CVarUse(decl = cr.cvar_limit)
        step = CVarUse(decl = cr.cvar_step)
        block = CVarUse(decl = cr.cvar_block)

        if self.newisa:
            strategyuse = CVarUse(cr.cvar_strategy)
            newbl += (flatten(cr.loc,
                          '__asm__ __volatile__("%s %%2, %%0\\t! MT: CREATE %s"'
                          ' : "=r"(' % (allocinsn,lbl)) + 
                  usefvar + ') : "0"(' + CVarUse(decl = cr.cvar_place) + 
                  '), "rP"(' + strategyuse + '));')
        else:
            newbl += (flatten(cr.loc,
                          '__asm__ __volatile__("%s %%0\\t! MT: CREATE %s"'
                          ' : "=r"(' % (allocinsn,lbl)) + 
                  usefvar + '));')

        if lc.target_next is not None:
            if self.newisa:
                failval = 0
            else:
                failval = -1

            newbl += (flatten(cr.loc, ' if (__builtin_expect(%d == (' % failval) + 
                          usefvar + '), 0)) ' + 
                          CGoto(target = lc.target_next)) + ';'
        
        newbl += (flatten(cr.loc, 
                         '__asm__ ("setstart %%0, %%2\\t! MT: CREATE %s"'
                         ' : "=r"(' % lbl) +
                  usefvar + ') : "0"(' + usefvar + '), "rP"(' + start + ')); '
                  '__asm__ ("setstep %%0, %%2\\t! MT: CREATE %s"' % lbl +
                  ' : "=r"(' + usefvar + ') : "0"(' + usefvar + '), "rP"(' + step + ')); ' 
                  '__asm__ ("setblock %%0, %%2\\t! MT: CREATE %s"' % lbl +
                  ' : "=r"(' + usefvar + ') : "0"(' + usefvar + '), "rP"(' + block + ')); ')
        if self.newisa:
              newbl += (Opaque('__asm__ ("setlimit %%0, %%2\\t! MT: CREATE %s"' % lbl) +
                  ' : "=r"(' + usefvar + ') : "0"(' + usefvar  + '), "rP"(' + limit + ')); ')
        else: # not self.newisa:
            # FIXME: this "-1" business is an ugly hack : uT-LEON3 was based on a screwed up
            # simulator source which used inclusive limits.
            newbl += (Opaque('__asm__ ("setlimit %%0, %%2\\t! MT: CREATE %s"' % lbl) +
                      ' : "=r"(' + usefvar + ') : "0"(' + usefvar  + '), "rP"((' + limit + ')-1)); '
                      '__asm__ ("setthread %%0, %%2\\t! MT: CREATE %s"' % lbl +
                      ' : "=r"(' + usefvar + ') : "0"(' + usefvar + '), "rP"(' + funvar + ')); ')

        # Now handle body between create..sync

        newbl += lc.body.accept(self)

        # On to the real stuff.
        if cr.extras.has_attr('extra'):
            if self.newisa:
                die("sl__extra not supported on this target", cr)
            xtravar = CVarUse(decl = cr.cvar_extra)
            newbl += (Opaque('__asm__ ("setarg %%0, %%2\\t! MT: CREATE %s"' % lbl) +
                      ' : "=r"(' + usefvar + ') : "0"(' + usefvar  + '), "rP"(' + xtravar + ')); ')

        argregs = set()
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
                           reg = (not self.newisa) and r or None)
            crc.decls += var
            gargs.append(CVarUse(decl = var))
            aregn += 1
            argregs.add(r)

        if c['gl_mem_offset'] is not None:
            # one extra global var
            r = regmagic.vname_to_legacy("l%d" % aregn)
            var = CVarDecl(loc = cr.loc,
                           name = 'C$aR$%s$%s' % (cr.label, name),
                           ctype = CTypeUse(tdecl=matype) + Opaque('*'),
                           init = Opaque(text = '&') + mavar,
                           reg = (not self.newisa) and r or None)
            crc.decls += var
            gargs.append(CVarUse(decl = var))
            aregn += 1
            argregs.add(r)

        faregn = 0
        fgargs = []
        for g in c['gfslots']:
            name = g['name']
            r = regmagic.vname_to_legacy("lf%d" % faregn)
            var = CVarDecl(loc = cr.loc, 
                           name = 'C$aR$%s$%s' % (cr.label, name), 
                           ctype = g['ctype'],
                           init = CVarUse(decl = cr.arg_dic[name].cvar),
                           reg = (not self.newisa) and r or None)
            crc.decls += var
            fgargs.append(CVarUse(decl = var))
            faregn += 1
            argregs.add(r)

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
                           reg = (not self.newisa) and r or None)
            crc.decls += var
            sargs.append(CVarUse(decl = var))
            collect += CVarSet(decl = arg_cvar, rhs = CVarUse(decl = var)) + Opaque(';')
            aregn += 1
            argregs.add(r)

        fsargs = []
        for s in c['sfslots']:
            name = s['name']
            r = regmagic.vname_to_legacy("lf%d" % faregn)
            arg_cvar = cr.arg_dic[name].cvar
            var = CVarDecl(loc = cr.loc, 
                           name = 'C$aR$%s$%s' % (cr.label, name), 
                           ctype = s['ctype'],
                           init = CVarUse(decl = arg_cvar),
                           reg = (not self.newisa) and r or None)
            crc.decls += var
            fsargs.append(CVarUse(decl = var))
            collect += CVarSet(decl = arg_cvar, rhs = CVarUse(decl = var)) + Opaque(';')
            faregn += 1
            argregs.add(r)

        if not self.newisa:
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
            for v in fsargs:
                olist +=  Opaque(', "=f"(') + v + ')'
                ilist +=  Opaque(', "%d"(' % roff) + v + ')'
                roff += 1
            for v in gargs:
                ilist +=  Opaque(', "r"(') + v + ')'
            for v in fgargs:
                ilist +=  Opaque(', "f"(') + v + ')'

            if cr.sync_type == 'normal':
               crc += (flatten(cr.loc,
                            ' __asm__ __volatile__("create %%0, %%0\\t! MT: CREATE %s DRAIN(%s)'
                            '\\n\\tmov %%0, %%0\\t! MT: SYNC %s" : ' % (lbl, ','.join(argregs).replace('%','%%'), lbl)) +
                    olist + ' : ' + ilist + ' : "memory");')
            else: # detach
               if len(sargs) > 0 or len(fsargs) > 0 or len(gargs) > 0 or len(fgargs) > 0:
                   die("cannot pass global/shared argument with detached creates on this target")

               crc += (flatten(cr.loc,
                            ' __asm__ __volatile__("create %%0, %%%%g0\\t! MT: CREATE %s" : ' % lbl) +
                   olist + ' : ' + ilist + ' : "memory");')

        else:
            crc += (flatten(cr.loc_end,
                            ' __asm__ __volatile__("crei %%2, %%0\\t! MT: CREATE %s"' % lbl) +
                    ' : "=r"(' + usefvar + ') : "0"(' + usefvar + '),' + 
                    '   "r"(' + funvar + ') : "memory");')

            aoff = 0
            for a in gargs:
                crc += (flatten(cr.loc_end,
                                ' __asm__("putg %%2, %%0, %d\\t! MT: set sarg"'
                                ' : "=r"(' % aoff) +
                        usefvar + ') : "0"(' + usefvar + 
                        '), "r"(' + a + '));')
                aoff += 1
            aoff = 0
            for a in fgargs:
                crc += (flatten(cr.loc_end,
                                ' __asm__("fputg %%2, %%0, %d\\t! MT: set sarg"'
                                ' : "=r"(' % aoff) +
                        usefvar + ') : "0"(' + usefvar + 
                        '), "f"(' + a + '));')
                aoff += 1
            aoff = 0
            for a in sargs:
                crc += (flatten(cr.loc_end,
                                ' __asm__("puts %%2, %%0, %d\\t! MT: set shared"'
                                ' : "=r"(' % aoff) +
                        usefvar + ') : "0"(' + usefvar + 
                        '), "r"(' + a + '));')
                aoff += 1
            aoff = 0
            for a in fsargs:
                crc += (flatten(cr.loc_end,
                                ' __asm__("fputs %%2, %%0, %d\\t! MT: set shared"'
                                ' : "=r"(' % aoff) +
                        usefvar + ') : "0"(' + usefvar + 
                        '), "f"(' + a + '));')
                aoff += 1
                           
            if cr.sync_type == 'normal':
                crc += (flatten(cr.loc_end,
                                ' __asm__ __volatile__("sync %%0, %%1; '
                                ' mov %%1, %%1\\t! MT: SYNC %s"' % lbl) +
                        ' : "=r"(' + usefvar + '), "=r"(' + 
                        CVarUse(decl = cr.cvar_exitcode) +
                        ') : "0"(' + usefvar + ') : "memory");')
                        
                aoff = 0
                for a in sargs:
                    crc += (flatten(cr.loc_end,
                                    ' __asm__("gets %%0, %d, %%1; '
                                    ' mov %%1, %%1\\t! MT get shared"' % aoff) +
                            ' : "=r"(' + usefvar + '), "=r"(' + a + 
                            ') : "0"(' + usefvar + '));')
                aoff = 0
                for a in fsargs:
                    crc += (flatten(cr.loc_end,
                                    ' __asm__("fgets %%0, %d, %%1; '
                                    ' fmovs %%1, %%1\\t! MT get shared"' % aoff) +
                            ' : "=r"(' + usefvar + '), "=f"(' + a + 
                            ') : "0"(' + usefvar + '));')
            
            crc += (flatten(cr.loc_end,
                            ' __asm__ __volatile__("release %%0\\t! MT: SYNC %s"' % lbl) +
                    ' : : "r"(' + usefvar + '));')
            
        # Alias the shared arguments back
        crc += collect

        newbl += crc

        return newbl

class SSync_2_MTSSSync(DefaultVisitor):

    def __init__(self, newisa = False, *args, **kwargs):
        self.newisa = newisa
        super(SSync_2_MTSSSync, self).__init__(*args, **kwargs)

    def visit_spawnsync(self, ss):
        newbl = Block(loc = ss.loc)
        newbl += (flatten(ss.loc,
                        '__asm__ __volatile__("sync %%1, %%0; '
                        'mov %%0, %%%%g0\\t! MT: SYNC %s"' % ss.label) +
                ' : "=&r"(' + ss.rhs + ') : "r"(' + ss.rhs + ') : "memory");')
        if self.newisa:
            newbl += flatten(ss.loc, '__asm__ __volatile__("release %0" : : "r"(') + ss.rhs + '));'
        return newbl

__all__ = ['SSync_2_MTSSSync', 'Create_2_MTSCreate']
