from ..visitors import DefaultVisitor, flatten
from ..ast import LowCreate

class ReduceGetA(DefaultVisitor):
      def visit_geta(self, geta):
            return flatten(geta.loc, LowCreate.arg_var(geta.name))

__all__ = ['ReduceGetA']
