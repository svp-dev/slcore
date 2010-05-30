from ..visitors import DefaultVisitor, flatten
from ..ast import FunDecl, LowCreate
from ..msg import warn

#### Sequential transforms ####

class Create_2_Loop(DefaultVisitor):

    def __init__(self, *args, **kwargs):
        super(Create_2_Loop, self).__init__(*args, **kwargs)
        
    def visit_geta(self, geta):
        return flatten(geta.loc, LowCreate.arg_var(geta.name))
      
    def visit_seta(self, seta):
        b = seta.rhs.accept(self)
        return [flatten(seta.loc, "%s = " % LowCreate.arg_var(seta.name)), b]

    def visit_lowcreatearg(self, arg):
        # prepare proto and uses
        if arg.type.startswith("sh"):
            self.__callist += ", &%s" % LowCreate.arg_var(arg.name)
            self.__protolist += ", %s *" % arg.ctype
        else:
            self.__callist += ", %s" % LowCreate.arg_var(arg.name)
            self.__protolist += ", %s" % arg.ctype

    def visit_lowcreate(self, cr):
        self.__callist = ""
        self.__protolist = ""

        if cr.target_next is not None:
            warn("alternative %s not used (sequential execution always succeeds)" %
                 cr.target_next, cr)

        for a in cr.args:
            a.accept(self) # accumulate the call/protolists

        callist = self.__callist
        protolist = self.__protolist

        newbl = []
        lbl = cr.label

        # generate the function pointer
        if cr.funtype == cr.FUN_ID:
            funvar = cr.fun
        else:
            funvar = '__slC_seqfun_%s' % lbl
            newbl.append(flatten(cr.loc, 
                                 " long (*%s)(const long %s) = "
                                 "(long (*)(const long%s))(%s);" 
                                 % (funvar, protolist, 
                                    protolist, cr.fun)))

        # consume body
        newbl.append(cr.body.accept(self))

        # here we expand the loop
        indexvar = "__slC_ix_%s" % lbl
        newbl.append(flatten(cr.loc_end, 
                             "long %(idx)s; "
                             "if (!%(step)s)"
                             "  for (%(idx)s = %(start)s; ;"
                             "       %(idx)s += %(limit)s)"
                             "  { if (0 != (%(retval)s ="
                             "    %(fun)s(%(idx)s%(callist)s)))"
                             "    break; } "
                             "else if (%(step)s > 0)"
                             "  for (%(idx)s = %(start)s;"
                             "       %(idx)s < %(limit)s;"
                             "       %(idx)s += %(step)s)"
                             "  { if (0 != (%(retval)s ="
                             "    %(fun)s(%(idx)s%(callist)s)))"
                             "    break; } "
                             "else"
                             "  for (%(idx)s = %(start)s;"
                             "       %(idx)s > %(limit)s;"
                             "       %(idx)s += %(step)s)"
                             "  { if (0 != (%(retval)s ="
                             "    %(fun)s(%(idx)s%(callist)s)))"
                             "    break; }; "
                             % { 'callist' : callist,
                                 'idx' : indexvar,
                                 'fun' : funvar,
                                 'start' : cr.start,
                                 'limit' : cr.limit,
                                 'step' : cr.step,
                                 'retval' : cr.retval }))

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
            format = "__slP_%s"
        return flatten(getp.loc, format % getp.name)

    def visit_setp(self, getp):
        b = getp.rhs.accept(self)
        if getp.name in self.__shlist:
            format = "(*__slP_%s) = "
        else:
            format = "__slP_%s = "
        return [flatten(getp.loc, format % getp.name), b]

    def visit_funparm(self, parm):
        if parm.type.startswith("sh"):
            self.__shlist.append(parm.name)
            self.__buffer += ', register %s * const __restrict__ __slP_%s ' \
                % (parm.ctype, parm.name)
        else:
            self.__gllist.append(parm.name)
            if parm.type.endswith('_mutable'):
                const = ""
            else:
                const = "const"
            self.__buffer += ', register %s %s __slP_%s ' \
                % (parm.ctype, const, parm.name)
        return parm

    def visit_fundecl(self, fundecl, keep = False):
        self.__shlist = []
        self.__gllist = []
        self.__buffer = "long %s(const long __slI" % fundecl.name
        for parm in fundecl.parms:
            parm.accept(self)
        self.__buffer += ')'
        ret = flatten(fundecl.loc, self.__buffer)
        self.__buffer = None
        if not keep:
            self.__shlist = self.__gllist = None
        return ret

    def visit_fundef(self, fundef):
        newitems = []
        newitems.append(self.dispatch(fundef, seen_as = FunDecl, keep = True))
        newitems.append(flatten(fundef.loc, "{"))
        newitems.append(fundef.body.accept(self))
        newitems.append(flatten(fundef.loc_end, " return 0; }"))
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
