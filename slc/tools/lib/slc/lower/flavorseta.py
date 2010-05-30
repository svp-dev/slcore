from ..visitors import ScopedVisitor, flatten
from ..ast import *


class FlavorSetA(ScopedVisitor):
    
    def __init__(self, *args, **kwargs):
        super(FlavorSetA, self).__init__(*args, **kwargs)
        self.lc_dic = {}

    def visit_lowcreate(self, lc):
        self.lc_dic[lc.label] = lc
        super(FlavorSetA, self).visit_lowcreate(lc)
        del self.lc_dic[lc.label]
        return lc

    def visit_seta(self, seta):
        #print "IN SETA: %r" % seta.__dict__
        seta.rhs.accept(self)
        lbl = seta.decl.create.label
        lc = self.lc_dic[lbl]
        seta.lowcreate = lc
        return Flavor(lc.flavor, items = seta)

__all__ = ['FlavorSetA']
