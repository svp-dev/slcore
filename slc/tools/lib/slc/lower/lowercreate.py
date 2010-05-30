

from ..visitors import DefaultVisitor, flatten
from ..ast import *

class Create_2_LowCreate(DefaultVisitor):

    def visit_create(self, cr):

        lc = LowCreate(loc = cr.loc,
                       loc_end = cr.loc_end,
                       label = cr.label,
                       sync_type = cr.sync_type)
        
        newbl = []

        newbl.append(flatten(cr.loc, 
                             "long const %s = " % lc.place))
        newbl.append(cr.place.accept(self))
        newbl.append(flatten(cr.loc, 
                             "; "
                             "long const %s = " % lc.start))
        newbl.append(cr.start.accept(self))
        newbl.append(flatten(cr.loc,
                             "; "
                             "long const %s = " % lc.limit))
        newbl.append(cr.limit.accept(self))
        newbl.append(flatten(cr.loc,
                             "; "
                             "long const %s = " % lc.step))
        newbl.append(cr.step.accept(self))
        newbl.append(flatten(cr.loc,
                             "; "
                             "long const %s = " % lc.block))
        newbl.append(cr.block.accept(self))

        newbl.append(flatten(cr.loc, ";"))

        if cr.funIsIdentifier():
            lc.funtype = lc.FUN_ID
            lc.fun = cr.fun
        else:
            lc.funtype = lc.FUN_PTR
            newbl.append(flatten(cr.loc,
                                 "void * const %s = (void*)(" % lc.fun))
            newbl.append(cr.fun.accept(self))
            newbl.append(flatten(cr.loc, ");"))

        lcas = []
        newbody = Block(loc = cr.body.loc, loc_end = cr.body.loc_end)
        for a in cr.args:
            lca = LowCreateArg(loc = a.loc,
                               name = a.name,
                               type = a.type,
                               ctype = a.ctype)
            newbl.append(flatten(a.loc, "%s %s;" % (a.ctype, lc.arg_var(a.name))))

            if a.init is not None:
                ininame = lc.arg_init_var(a.name)

                newbl.append(flatten(a.loc, 
                                     "%s const %s = " 
                                     % (a.ctype, ininame)))
                newbl.append(a.init.accept(self))
                newbl.append(flatten(a.loc, ";"))

                newbody += [flatten(a.loc, ";"),
                            SetA(loc = a.loc, name = a.name, 
                                 rhs = Block(items = [Opaque(ininame)]))]
            lcas.append(lca)

        lc.args = lcas
        lc.body = newbody
        lc.body += cr.body.accept(self)

        newbl.append(flatten(cr.loc, "long %s;" % lc.retval))

        newbl.append(lc)

        newbl.append(flatten(cr.loc_end, "; %s: (void)0 " % lc.target_resolved))

        if cr.result_lvalue is not None:
            newbl.append(flatten(cr.loc_end, "; ("))
            newbl.append(cr.result_lvalue.accept(self))
            newbl.append(flatten(cr.loc_end, ") = %s" % lc.retval))

        return newbl
                         
        
