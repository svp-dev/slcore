from ...visitors import DefaultVisitor, ScopedVisitor, flatten
from ...ast import *
from ...msg import warn,die
from regdefs import regmagic

class Create_2_L2MTCreate(ScopedVisitor):

    def visit_seta(self, seta):
        b = seta.rhs.accept(self)
        
        name = seta.name

        adic = seta.lowcreate.callconv
        gblvar = seta.lowcreate.gblvar
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
            assign = CVarSet(decl = seta.decl.cvar, rhs = b)
            insn = 'r_write'
            if a['species'] == 'f': 
                insn = 'f' + insn
                rspec = 'f'
            else: rspec = 'rI'
            regnr = a['regnr']
            newbl.append(flatten(seta.loc, 
                                 ' __asm__ ("%(insn)s %%0, %%4\\t! MT: set %(cat)sarg"'
                                 ' : "=r"(' % locals()) +
                         gblvar + '), "=r"(' + fidvar + ') : "0"(' + gblvar + '+%d' % regnr +
                         '), "1"(' + fidvar + '), "%s"((' % rspec + ctype + ')(' +
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
        fidvar = cr.cvar_fid

        has_globals = False
        nr_globals = c['nrargregs']['gli']
        if nr_globals > 0:
            has_globals = True
            cr.cvar_rb = CVarDecl(loc = cr.loc, name = 'C$Fb$%s' % cr.label, ctype = 'long')
            self.cur_scope.decls += cr.cvar_rb
            gblvar = CVarUse(decl = cr.cvar_rb)
            lc.gblvar = gblvar
            
        start = CVarUse(decl = cr.cvar_start)
        limit = CVarUse(decl = cr.cvar_limit)
        step = CVarUse(decl = cr.cvar_step)
        block = CVarUse(decl = cr.cvar_block)
        
        usefvar = CVarUse(decl = fidvar)

        newbl += (flatten(cr.loc,
                          '__asm__ __volatile__("f_alloc %%0\\t! MT: CREATE %s"'
                          ' : "=r"(' % lbl) + 
                  usefvar + '));')
        
        if lc.target_next is not None:
            newbl += (flatten(cr.loc, ' if (!__builtin_expect(!!(') + 
                      usefvar + '), 1)) ' + 
                      CGoto(target = lc.target_next)) + ';'

        if has_globals:
            mask = 0
            if nr_globals > 4:
                mask = 7
            elif nr_globals > 2:
                mask = 3
            else:
                mask = 1
            newbl += (flatten(cr.loc,
                              '__asm__ __volatile__("r_allocsrb %%1, %%0\\t! MT: CREATE %s"'
                              ' : "=r"(' % lbl) + 
                              gblvar + ') : "rI"(%d));' % mask)
            if lc.target_next is not None:
                newbl += (flatten(cr.loc, ' if (!__builtin_expect(!!(') + 
                          gblvar + '), 1)) {' +
                          '__asm__ __volatile__("f_fence %%0, 31\\t! MT: ABORT1 CREATE %s" : : "r"(' % lbl + usefvar + '));' + 
                          CGoto(target = lc.target_next) + ';};')
            newbl += (flatten(cr.loc,
                             '__asm__ ("f_mapg %%0, %%2\\t! MT: CREATE %s" : "=r"(' % lbl) +
                      usefvar + ') : "r"(' + usefvar + '), "r"(' + gblvar + '));')
                
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

        # prepare the thread group
        tgname = "C$htg$%s" % lbl
        tgdecl = CVarDecl(loc = cr.loc, name = tgname, ctype = 'long')
        tgvar = CVarUse(decl = tgdecl)
        self.cur_scope.decls += tgdecl

        newbl += (flatten(cr.loc, ' if (!__builtin_expect(!!(') + 
                  block + '), 1)) ' + block + ' = -1;')

        newbl += (flatten(cr.loc,
                          '__asm__ ("t_allochtg %%1, %%0, %%0\\t! MT: CREATE %s FUN %%2 ' % lbl) + funvar +
                  '": "=&r"(' + tgvar + ') : "r"(' + block + '), ' + 
                  '"r"(' + funvar + '));')
        if lc.target_next is not None:
            newbl += (flatten(cr.loc, ' if (!__builtin_expect(!!(') + 
                      tgvar + '), 1)) {' +
                      '__asm__ __volatile__("f_fence %%0, 31\\t! MT: ABORT2 CREATE %s" : : "r"(' % lbl + usefvar + '));' + 
                      CGoto(target = lc.target_next) + ';};')
        newbl += (flatten(cr.loc,
                          '__asm__ ("f_maphtg %%0, %%2\\t! MT: CREATE %s"' % lbl) +
                  ': "=r"(' + usefvar + ') : "r"(' + usefvar + '), "r"(' + tgvar + '));')
            
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
        newbl += (flatten(cr.loc, 
                          '__asm__ ("f_set_blocksize %%0, %%2\\t! MT: CREATE %s"'
                         ' : "=r"(' % lbl) +
                  usefvar + ') : "0"(' + usefvar + '), "rI"(' + limit + ')); ' +
                  '__asm__ ("f_set_gridsize %%0, %%2\\t! MT: CREATE %s"' % lbl +
                  ' : "=r"(' + usefvar + ') : "0"(' + usefvar + '), "rI"(' + step + ')); ' )

        lc.callconv = c['nargs']
        lc.fidvar = usefvar

        newbl += lc.body.accept(self)


        # done with body, now handle sync

        # first of all, if there weresome memory-passed arguments,
        # we need to push the argument register to the child family.
        # A memory barrier is required because the remote thread(s) may
        # access the memory as soon as r_write completes.
        
        if c['gl_mem_offset'] is not None:
            newbl += (flatten(cr.loc_end, 
                             ' __asm__ ("wmb; r_write %%2, %%3\\t!MT: set offset for memargs in %%0"') + 
                      ' : "=r"(' + usefvar + ') : "0"(' + usefvar + '),' +
                      '   "r"(' + gblvar + '+%d)' % c['gl_mem_offset'] +
                      ', "r"(&' + mavar + '));')

        # actually create the family
        newbl += (flatten(cr.loc_end,
                          '__asm__ __volatile__("f_create %%0, %%2, %%0\\t! MT: CREATE %s"' % lbl) +
                  ' : "=r"(' + usefvar + ') : "0"(' + usefvar + '),' +
                  '   "r"(' + funvar + ') : "memory");')
        
        # now, on to the sync.
        if cr.sync_type == 'normal':
            # then wait for child family to terminate.
            newbl += (flatten(cr.loc_end, 
                              '__asm__ __volatile__("f_fence %%0, 31; nop;'
                              ' t_wait\\t! MT: SYNC %s"' % lbl) +
                      ' : "=r"(' + usefvar + ') : "0"(' + usefvar + ') : "memory");') 
        elif cr.sync_type == 'detach':
            # automatically release resources upon termination
            newbl += (flatten(cr.loc_end, 
                              '__asm__ __volatile__("f_fence %%0, 30; nop;') +
                      ' : "=r"(' + usefvar + ') : "0"(' + usefvar + '));')

        return newbl


class SSync_2_L2MTSSync(DefaultVisitor):

    def visit_spawnsync(self, ss):
        return (flatten(ss.loc, 
                        '__asm__ __volatile__("f_fence %%1, 31; nop; '
                        ' t_wait\\t! MT: SYNC %s"' % ss.label) +
                ' : "=r"(' + ss.rhs + ') : "0"(' + ss.rhs + ') : "memory");')
                

__all__ = ['Create_2_L2MTCreate', 'SSync_2_L2MTSSync']
