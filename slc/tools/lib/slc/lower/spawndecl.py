from ..visitors import DefaultVisitor
from ..ast import Opaque

class LowerSpawnDecl(DefaultVisitor):
    def visit_spawndecl(self, sd):
        return Opaque(' long %s ' % sd.spawnname)
    
        
