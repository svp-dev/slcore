from ..visitors import DefaultVisitor, flatten

class ReduceCCast(DefaultVisitor):

    def visit_ccast(self, cc):
        newbl = []
        newbl.append(flatten(cc.loc,'((%s)(' % cc.ctype))
        newbl.append(cc.expr.accept(self))
        newbl.append(flatten(None, '))'))
        return newbl

__all__ = ['ReduceCCast']
