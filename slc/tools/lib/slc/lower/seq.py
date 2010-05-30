from ..visitors import DefaultVisitor, flatten

#### Sequential transforms ####

class Create_2_Loop(DefaultVisitor):

    def __init__(self, *args, **kwargs):
        super(Create_2_Loop, self).__init__(*args, **kwargs)
        self.__decls = None
        self.__callist = None
        self.__protolist = None
        
    def visit_geta(self, geta):
        return flatten(geta.loc, "__slA_%s" % geta.name)
      
    def visit_seta(self, seta):
        b = seta.rhs.accept(self)
        return [flatten(seta.loc, "__slA_%s = " % seta.name), b]

    def visit_lowcreatearg(self, arg):
        # prepare declarations
        if arg.type.startswith('gl'):
            reg = "register"
        else:
            reg = ""

        self.__decls.append(flatten(arg.loc, "%s %s __slA_%s;" 
                                    % (reg, arg.ctype, arg.name)))
        # prepare proto and uses
        if arg.type.startswith("sh"):
            self.__callist += ", &__slA_%s" % arg.name
            self.__protolist += ", %s *" % arg.ctype
        else:
            self.__callist += ", __slA_%s" % arg.name
            self.__protolist += ", %s" % arg.ctype

    def visit_lowcreate(self, cr):
        old_decls = self.__decls  # allow for nested creates
        old_callist = self.__callist 
        old_protolist = self.__protolist
        self.__decls = []
        self.__callist = ""
        self.__protolist = ""

        for a in cr.args:
            a.accept(self) # accumulate the declarations in self.decls

        newbl = []
        lbl = cr.label

        # generate the function pointer
        funvar = '__slC_seqfun_%s' % lbl
        newbl.append(flatten(cr.loc, 
                             "long (*%s)(const long %s) = "
                             "(long (*)(const long%s))(%s);" 
                             % (funvar, self.__protolist, 
                                self.__protolist, cr.fun)))

        # consume argument declarations 
        newbl += self.__decls 

        # consume body
        newbl.append(cr.body.accept(self))

        # here we expand the loop
        indexvar = "__slC_ix_%s" % lbl
        newbl.append(flatten(cr.loc_end, 
                             "register long %(idx)s; "
                             "if (!%(step)s)"
                             "  for (%(idx)s = %(start)s; ;"
                             "       %(idx)s += %(limit)s)"
                             "  { if (SVP_ENORMAL != (%(retval)s ="
                             "    %(fun)s(%(idx)s%(callist)s)))"
                             "    break; } "
                             "else if (%(step)s > 0)"
                             "  for (%(idx)s = %(start)s;"
                             "       %(idx)s < %(limit)s;"
                             "       %(idx)s += %(step)s)"
                             "  { if (SVP_ENORMAL != (%(retval)s ="
                             "    %(fun)s(%(idx)s%(callist)s)))"
                             "    break; } "
                             "else"
                             "  for (%(idx)s = %(start)s;"
                             "       %(idx)s > %(limit)s;"
                             "       %(idx)s += %(step)s)"
                             "  { if (SVP_ENORMAL != (%(retval)s ="
                             "    %(fun)s(%(idx)s%(callist)s)))"
                             "    break; }"
                             % { 'callist' : self.__callist,
                                 'idx' : indexvar,
                                 'fun' : funvar,
                                 'start' : cr.start,
                                 'limit' : cr.limit,
                                 'step' : cr.step,
                                 'retval' : cr.retval }))

        self.__decls = old_decls
        self.__callist = old_callist
        self.__protolist = old_protolist
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
            self.__buffer += ', register %s * const __restrict__ __slP_%s' \
                % (parm.ctype, parm.name)
        else:
            self.__gllist.append(parm.name)
            if parm.type.endswith('_mutable'):
                const = ""
            else:
                const = "const"
            self.__buffer += ', register %s %s __slP_%s' \
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
        newitems.append(self.visit_fundecl(fundef, keep = True))
        newitems.append(flatten(fundef.loc, "{"))
        if fundef.body.indexname is not None:
            newitems.append(flatten(fundef.loc, 
                                    "register const long %s = __slI;" 
                                    % fundef.body.indexname)) 
        b = fundef.body.accept(self)
        b.indexname = None 
        newitems.append(b)
        newitems.append(flatten(fundef.loc_end, "return SVP_ENORMAL; }"))
        return newitems

    def visit_break(self, br):
        return flatten(br.loc, "return SVP_EBROKEN")

    def visit_endthread(self, et):
        return flatten(et.loc, "return SVP_ENORMAL")


__all__ = ['Create_2_Loop', 'TFun_2_CFun']
