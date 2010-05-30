from ..visitors import DefaultVisitor, flatten
from ..ast import LowCreate

class ReduceCVars(DefaultVisitor):

    def visit_cvaruse(self, vu):
        return flatten(vu.loc, vu.decl.name)

    def visit_cvarset(self, vs):
        vs.rhs.accept(self)

        newbl = []
        newbl.append(flatten(vs.loc, "%s = (" % vs.decl.name))
        newbl.append(vs.rhs)
        newbl.append(flatten(None, ")"))
        return newbl

    def visit_cvardecl(self, vd):
        vd.init.accept(self)
        
        if len(vd.init) == 0:
            return flatten(vd.loc, "%s %s;" % (vd.ctype, vd.name))
        else:
            newbl = []
            newbl.append(flatten(vs.loc, "%s %s = (" % (vd.ctype, vd.name)))
            newbl.append(vd.init)
            newbl.append(flatten(None, ");"))
            return newbl

__all__ = ['ReduceCVars']
