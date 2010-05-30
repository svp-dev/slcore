from ..visitors import DefaultVisitor, flatten
from ..ast import LowCreate

class ReduceCLabels(DefaultVisitor):

    def visit_clabel(self, cl):
        return flatten(cl.loc, " %s: (void)0;" % cl.name)

    def visit_cgoto(self, gt):
        return flatten(gt.loc, " goto %s " % gt.target.name)

__all__ = ['ReduceCLabels']
