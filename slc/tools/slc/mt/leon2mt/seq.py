from ...visitors import DefaultVisitor, ScopedVisitor, flatten
from ...ast import *
from ...msg import warn
from ...lower.seq import Create_2_Loop, TFun_2_CFun

#### Sequential transforms ####

class Create_2_L2MTLoop(Create_2_Loop):

    def __init__(self, *args, **kwargs):
        super(Create_2_L2MTLoop, self).__init__(*args, **kwargs)

    def visit_lowcreate(self, lc):
        
        cr = self.cur_scope.creates[lc.label]

        self.callist = []
        self.protolist = Block()

        if lc.target_next is not None:
            warn("alternative %s not used (sequential execution always succeeds)" %
                 lc.target_next.name, lc)

        for a in cr.args:
            a.accept(self) # accumulate the call/protolists

        callist = self.callist
        protolist = self.protolist

        newbl = []
        lbl = cr.label
        fidvar = cr.cvar_fid

        # generate the function pointer
        if cr.funtype == cr.FUN_ID:
            if lc.lowfun is not None:
                funvar = lc.lowfun
            else:
                # not yet split
                funvar = Opaque(cr.fun)
        else:
            assert cr.funtype == cr.FUN_VAR

            n = 'C$SF$%s' % lbl
            t = 'C$SF$%s' % lbl
            thetype = CTypeDecl(loc = cr.loc,
                                name = t,
                                ctype = CType(items = 
                                              Opaque(text = "long (*") +
                                              CTypeHead() + 
                                              ')(const long' + protolist + ')'))
            self.cur_scope.decls += thetype
            funvar = CVarDecl(loc = cr.loc, name = n, ctype = CTypeUse(tdecl = thetype))
            self.cur_scope.decls += funvar

            if lc.lowfun is not None:
                thefun = lc.lowfun
            else:
                # not yet split
                thefun = CVarUse(decl = cr.fun)

            newbl.append(CVarSet(loc = cr.loc, 
                                 decl = funvar,
                                 rhs = CCast(ctype = CTypeUse(tdecl = thetype), 
                                             expr = thefun)))

            funvar = CVarUse(decl = funvar)

        # generate pseudo-FID
        newbl.append(Opaque(';') + CVarSet(decl = fidvar, rhs = Opaque('0')))

        # consume body
        newbl.append(lc.body.accept(self))

        # here we expand the loop
        indexvar_x = CVarDecl(loc = cr.loc_end, name = 'C$Six$%s' % lbl, ctype = 'unsigned short')
        self.cur_scope.decls += indexvar_x
        indexvar_y = CVarDecl(loc = cr.loc_end, name = 'C$Siy$%s' % lbl, ctype = 'unsigned short')
        self.cur_scope.decls += indexvar_y
        indexvar = CVarDecl(loc = cr.loc_end, name = 'C$Si$%s' % lbl, ctype = 'unsigned long')
        self.cur_scope.decls += indexvar
        blockvar_x = CVarDecl(loc = cr.loc_end, name = 'C$Sbx$%s' % lbl, ctype = 'unsigned short')
        self.cur_scope.decls += blockvar_x
        blockvar_y = CVarDecl(loc = cr.loc_end, name = 'C$Sby$%s' % lbl, ctype = 'unsigned short')
        self.cur_scope.decls += blockvar_y
        blockvar = CVarDecl(loc = cr.loc_end, name = 'C$Sb$%s' % lbl, ctype = 'unsigned long')
        self.cur_scope.decls += blockvar
        
        ix = CVarUse(decl = indexvar_x)
        iy = CVarUse(decl = indexvar_y)
        iv = CVarUse(decl = indexvar)
        bx = CVarUse(decl = blockvar_x)
        by = CVarUse(decl = blockvar_y)
        bv = CVarUse(decl = blockvar)

        endloop = CLabel(loc = cr.loc_end, name = 'Cle$%s' % cr.label)
        
        limit = CVarUse(decl = cr.cvar_limit)
        step = CVarUse(decl = cr.cvar_step)
        docall = CVarSet(decl = cr.cvar_exitcode,
                         rhs = funvar + '(' + iv + callist + ',' + bv + ',' + limit + ',' + step + ')')

        newbl.append(flatten(cr.loc_end,
                             ' if((') + limit + ' >> 9) == 0) ' + limit + ' |= 0x200;' +
                     'if ((' + step + ' >>9) == 0) ' + step + ' |= 0x200;')
        newbl.append(flatten(cr.loc_end, 
                             "for (") + by + " = 0; " + by + " < (" + step + ">>9); " + by + "++)" +
                     "for (" + bx + " = 0; " + bx + " < (" + step + "&0x1ff); " + bx + "++) { " +
                     bv + " = ((unsigned)" + by + "<<9) | " + bx + ";" +
                     "for (" + iy + " = 0; " + iy + " < (" + limit + ">>9); " + iy + "++)" +
                     "for (" + ix + " = 0; " + ix + " < (" + limit + "&0x1ff); " + ix + "++) {" +
                     iv + " = ((unsigned)" + iy + "<<9) | " + ix + ";" +
                     "if (0 != (" + docall + ")) " + CGoto(loc = cr.loc_end, target = endloop) + "; }}" + endloop)
        
        return newbl


class TFun_2_L2MTCFun(TFun_2_CFun):

    def __init__(self, *args, **kwargs):
        super(TFun_2_L2MTCFun, self).__init__(*args, **kwargs)
        self.shlist = None
        self.gllist = None

    def visit_fundecl(self, fundecl, infundef = False):
        old_shlist = self.shlist
        old_gllist = self.gllist
        self.shlist = []
        self.gllist = []
        qual = ""
        iattr = ""
        if fundecl.extras.get_attr('static', None) is not None:
            qual = "static"
        elif infundef:
            iattr = " __attribute__((unused))"
        else:
            qual = "extern"
        self.buffer = flatten(fundecl.loc, 
                                " %s long %s(const unsigned long __slI%s" 
                                % (qual, fundecl.name, iattr))
        for parm in fundecl.parms:
            parm.accept(self)
        self.buffer += flatten(fundecl.loc,
                                 ', const unsigned long __slB%s, const unsigned long __slBS%s, const unsigned long __slGS%s)' % (iattr, iattr, iattr))
        ret = self.buffer
        self.buffer = None
        if not infundef:
            self.shlist = old_shlist
            self.gllist = old_gllist
        return ret
 
    def visit_fundeclptr(self, fundecl):
        old_shlist = self.shlist
        old_gllist = self.gllist
        self.shlist = []
        self.gllist = []
        if fundecl.extras.get_attr('typedef', None) is not None:
            qual = "typedef"
        elif fundecl.extras.get_attr('static', None) is not None:
            qual = "static"
        else:
            qual = ''
        self.buffer = flatten(fundecl.loc, 
                                " %s long (*%s)(const long __slI" 
                                % (qual, fundecl.name))
                         
        for parm in fundecl.parms:
            parm.accept(self)
        self.buffer += flatten(fundecl,loc,
                                 ', const long __slB, const long __slBS, const long __slGS)')
        ret = self.buffer
        self.buffer = None
        self.shlist = old_shlist
        self.gllist = old_gllist
        return ret

    def visit_indexdecl(self, idecl):
        b = []
        b.append(super(TFun_2_L2MTCFun, self).visit_indexdecl(idecl))
        b.append(flatten(idecl.loc,
                         '; register union { struct { unsigned y : 9; unsigned x : 9; }; unsigned __slI; } threadIdx; threadIdx.__slI = __slI;'
                         ' register union { struct { unsigned y : 9; unsigned x : 9; }; unsigned __slB; } blockIdx; blockIdx.__slB = __slB;'
                         ' register union { struct { unsigned y : 9; unsigned x : 9; }; unsigned __slS; } blockDim; blockDim.__slS = __slBS;'
                         ' register union { struct { unsigned y : 9; unsigned x : 9; }; unsigned __slS; } gridDim; gridDim.__slS = __slGS'))
        return b


__all__ = ['Create_2_L2MTLoop', 'TFun_2_L2MTCFun']
