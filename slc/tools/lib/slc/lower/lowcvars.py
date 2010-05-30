from ..visitors import DefaultVisitor, flatten
from ..ast import *
import copy

def decapsulate_ctype(ty):
    return Block(items = copy.copy(ty._items))

class ReduceCVars(DefaultVisitor):

    def __init__(self, *args, **kwargs):
        super(ReduceCVars, self).__init__(*args, **kwargs)
        self.cur_ctypename = None

    def visit_cvaruse(self, vu):
        return flatten(vu.loc, vu.decl.name)

    def visit_ctypeuse(self, vu):
        return flatten(vu.loc, vu.tdecl.name)

    def visit_cvarset(self, vs):
        vs.rhs.accept(self)

        newbl = []
        newbl.append(flatten(vs.loc, "%s = (" % vs.decl.name))
        newbl.append(vs.rhs)
        newbl.append(flatten(None, ")"))
        return newbl

    def visit_ctypehead(self, cth):
        if self.cur_ctypename is None:
            # not interested in this one, keep it
            return cth
        n = self.cur_ctypename
        self.cur_ctypename = None
        return Opaque(text = n)

    def visit_cvardecl(self, vd):
        self.cur_ctypename = vd.name;
        vd.ctype.accept(self)
        t = decapsulate_ctype(vd.ctype)

        if self.cur_ctypename is not None:
            # no head, append name
            t += Opaque(text = ' %s' % vd.name)
            self.cur_ctypename = None

        newbl = flatten(vd.loc, ' ') + t

        vd.init.accept(self)       
        if len(vd.init) > 0:
            newbl += Opaque(text = ' = (') + vd.init + ')'

        newbl += ';'
        return newbl

    def visit_ctypedecl(self, vd):
        self.cur_ctypename = vd.name;
        vd.ctype.accept(self)
        t = decapsulate_ctype(vd.ctype)

        if self.cur_ctypename is not None:
            # no head, append name
            t += Opaque(text = ' %s' % vd.name)
            self.cur_ctypename = None

        newbl = flatten(vd.loc, ' typedef ') + t + ';'
        return newbl
    

__all__ = ['ReduceCVars']
