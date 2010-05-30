from ..visitors import DefaultVisitor, ScopedVisitor, flatten
from ..ast import *
from ..msg import warn

#### Sequential transforms ####

class Create_2_Loop(ScopedVisitor):

    def __init__(self, *args, **kwargs):
        super(Create_2_Loop, self).__init__(*args, **kwargs)
      
    def visit_seta(self, seta):
        b = seta.rhs.accept(self)
        return CVarSet(loc = seta.loc, decl = seta.decl.cvar, rhs = b) 

    def visit_createarg(self, arg):
        # prepare proto and uses
        if arg.type.startswith("sh"):
            self.__callist.append(flatten(None, ', &'))
            self.__callist.append(CVarUse(decl = arg.cvar))
            self.__protolist += flatten(None, ', ') + arg.ctype + flatten(None, ' *')
        else:
            self.__callist.append(flatten(None, ', '))
            self.__callist.append(CVarUse(decl = arg.cvar))
            self.__protolist += flatten(None, ', ') + arg.ctype
        return arg

    def visit_lowcreate(self, lc):
        
        cr = self.cur_scope.creates[lc.label]

        self.__callist = []
        self.__protolist = Block()

        if lc.target_next is not None:
            warn("alternative %s not used (sequential execution always succeeds)" %
                 lc.target_next.name, lc)

        for a in cr.args:
            a.accept(self) # accumulate the call/protolists

        callist = self.__callist
        protolist = self.__protolist

        newbl = []
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

        # consume body
        newbl.append(lc.body.accept(self))

        # here we expand the loop
        indexvar = CVarDecl(loc = cr.loc_end, name = 'C$Si$%s' % lbl, ctype = 'long')
        self.cur_scope.decls += indexvar

        ix = CVarUse(decl = indexvar)
        start = CVarUse(decl = cr.cvar_start)
        limit = CVarUse(decl = cr.cvar_limit)
        step = CVarUse(decl = cr.cvar_step)
        docall = CVarSet(decl = cr.cvar_exitcode,
                         rhs = funvar + '(' + ix + callist + ')')

        newbl.append(flatten(cr.loc_end, "if (!") + step + ') ' +
                     "for (" + ix + " = " + start + '; ;' + ix + " += " + limit + ")" +
                     "{ if (0 != (" + docall + ')) break; }' +
                     "else if (" + step + " > 0) " + 
                     "for (" + ix + " = " + start + "; " + 
                     ix + " < " + limit + "; " +
                     ix + " += " + step + ") " +
                     "{ if (0 != (" + docall + ")) break; }" +
                     "else "
                     "for (" + ix + " = " + start + "; " + 
                     ix + " > " + limit + "; " +
                     ix + " += " + step + ") " +
                     "{ if (0 != (" + docall + ")) break; }" +
                     ";")

        return newbl


class TFun_2_CFun(DefaultVisitor):

    def __init__(self, *args, **kwargs):
        super(TFun_2_CFun, self).__init__(*args, **kwargs)
        self.__shlist = None
        self.__gllist = None

    def visit_getp(self, getp):
        if getp.name in self.__shlist:
            format = "(*__slP_%s)"
        else:
            format = " __slP_%s "
        return flatten(getp.loc, format % getp.name)

    def visit_setp(self, getp):
        b = getp.rhs.accept(self)
        if getp.name in self.__shlist:
            format = "(*__slP_%s) = "
        else:
            format = " __slP_%s = "
        return [flatten(getp.loc, format % getp.name), b]

    def visit_funparm(self, parm):
        if parm.type.startswith("sh"):
            self.__shlist.append(parm.name)
            self.__buffer += (Opaque(', register ') + parm.ctype + 
                              ' * const __restrict__ __slP_%s ' % parm.name)
        else:
            self.__gllist.append(parm.name)
            if parm.type.endswith('_mutable'):
                const = ""
            else:
                const = "const"
            self.__buffer += (Opaque(', register ') + parm.ctype + 
                              ' %s __slP_%s ' % (const, parm.name))
        return parm

    def visit_fundecl(self, fundecl, keep = False, omitextern = False):
        self.__shlist = []
        self.__gllist = []
        if fundecl.extras.get_attr('static', None) is not None:
            qual = "static"
        elif omitextern:
            qual = ""
        else:
            qual = "extern"
        self.__buffer = flatten(fundecl.loc, 
                                " %s long %s(const long __slI" 
                                % (qual, fundecl.name))
        for parm in fundecl.parms:
            parm.accept(self)
        self.__buffer += ')'
        ret = self.__buffer
        self.__buffer = None
        if not keep:
            self.__shlist = self.__gllist = None
        return ret
 
    def visit_fundeclptr(self, fundecl):
        self.__shlist = []
        self.__gllist = []
        if fundecl.extras.get_attr('typedef', None) is not None:
            qual = "typedef"
        elif fundecl.extras.get_attr('static', None) is not None:
            qual = "static"
        else:
            qual = ''
        self.__buffer = flatten(fundecl.loc, 
                                " %s long (*%s)(const long __slI" 
                                % (qual, fundecl.name))
        for parm in fundecl.parms:
            parm.accept(self)
        self.__buffer += ')'
        ret = self.__buffer
        self.__buffer = None
        self.__shlist = self.__gllist = None
        return ret

    def visit_fundef(self, fundef):
        newitems = self.dispatch(fundef, seen_as = FunDecl, keep = True, omitextern = True)
        newitems += flatten(fundef.loc, "{")
        newitems += fundef.body.accept(self)
        newitems += flatten(fundef.loc_end, " return 0; }")
        self.__shlist = self.__gllist = None
        return newitems

    def visit_indexdecl(self, idecl):
        return flatten(idecl.loc, 
                       " register const long %s = __slI " 
                       % idecl.indexname) 

    def visit_break(self, br):
        return flatten(br.loc, " return 1 ")

    def visit_endthread(self, et):
        return flatten(et.loc, " return 0 ")


__all__ = ['Create_2_Loop', 'TFun_2_CFun']
