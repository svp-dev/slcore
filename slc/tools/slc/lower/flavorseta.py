from ..visitors import DefaultVisitor, ScopedVisitor, flatten
from ..ast import *

class LinkSetA(ScopedVisitor):

    def __init__(self, *args, **kwargs):
        super(LinkSetA, self).__init__(*args, **kwargs)
        self.lc_dic = {}

    def visit_lowcreate(self, lc):
        self.lc_dic[lc.label] = lc
        super(LinkSetA, self).visit_lowcreate(lc)
        del self.lc_dic[lc.label]
        return lc

    def visit_seta(self, seta):
        #print "IN SETA: %r" % seta.__dict__
        seta.rhs.accept(self)
        lbl = seta.decl.create.label
        lc = self.lc_dic[lbl]
        seta.lowcreate = lc
        return seta

class FlavorSetA(DefaultVisitor):

    def visit_seta(self, seta):
        #print "IN SETA: %r" % seta.__dict__
        seta.rhs.accept(self)
        lc = seta.lowcreate
        return Flavor(lc.flavor, items = seta)

__all__ = ['LinkSetA', 'FlavorSetA']
