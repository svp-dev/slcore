from ..visitors import DefaultVisitor
from ..ast import Block

class RemoveFlavors(DefaultVisitor):

    def visit_flavor(self, fl):
        self.dispatch(fl, seen_as = Block)
        return fl._items

__all__ = ['RemoveFlavors']
