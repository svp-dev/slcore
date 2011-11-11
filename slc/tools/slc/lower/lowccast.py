from ..visitors import DefaultVisitor, flatten
from ..ast import *
from lowcvars import decapsulate_ctype

class ReduceCCast(DefaultVisitor):

    def __init__(self, *args, **kwargs):
        super(ReduceCCast, self).__init__(*args, **kwargs)
        self.cur_ctypename = None

    def visit_ctypehead(self, cth):
        if self.cur_ctypename is None:
            # not interested in this one, keep it
            return cth
        n = self.cur_ctypename
        self.cur_ctypename = None
        return Opaque(n)  

    def visit_ccast(self, cc):
        self.cur_ctypename = ''
        cc.ctype.accept(self)
        self.cur_ctypename = None
        
        newbl = (Opaque(loc = cc.loc, text = '((') + 
                 decapsulate_ctype(cc.ctype) + ')(' +
                 cc.expr.accept(self) +
                 '))')
        return newbl

__all__ = ['ReduceCCast']
