from ..visitors import DefaultVisitor, flatten
from ..ast import CVarUse

class ReduceGetA(DefaultVisitor):
      def visit_geta(self, geta):
            return CVarUse(loc = geta.loc, decl = geta.decl.cvar)

__all__ = ['ReduceGetA']
