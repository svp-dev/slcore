from ..visitors import ScopedVisitor, DefaultVisitor, flatten
from ..ast import *

class LowerSpawnSync(ScopedVisitor):

    def __init__(self, *args, **kwargs):
        super(LowerSpawnSync, self).__init__(*args, **kwargs)
        self.cnt = 1

    def visit_spawnsync(self, ss):

        ss.rhs.accept(self)
        
        cnt = self.cnt
        self.cnt += 1

        ss.label = '__Sp%d' % cnt

        self.cur_scope.decls += SpawnDecl(loc = ss.loc, spawnname = ss.label) + ';'

        newbl = (Opaque(" %s = (" % ss.label) + 
                 ss.rhs + 
                 ");")

        ss.rhs = Opaque(ss.label)
        ss.target_resolved = CLabel(loc = ss.loc, name = 'Se$%d' % cnt)
        newbl += ss + ';' + ss.target_resolved

        return newbl

class AutoResolveSpawnSync(DefaultVisitor):

    def visit_spawnsync(self, ss):
        return ss + ';' + CGoto(loc = ss.loc, target = ss.target_resolved)

__all__ = ["LowerSpawnSync", "AutoResolveSpawnSync"]
