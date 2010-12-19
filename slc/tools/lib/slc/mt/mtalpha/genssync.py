from ...visitors import DefaultVisitor, flatten

class SSync_2_MTASSync(DefaultVisitor):

    def visit_spawnsync(self, ss):
        return (flatten(ss.loc, 
                        '__asm__ __volatile__("sync %%1, %%0; '
                        'mov %%0, $31; '
                        'release %%1\\t# MT: SYNC %s"' % ss.label) +
                ' : "=&r"(' + ss.rhs + ') : "r"(' + ss.rhs + ') : "memory");')
                

__all__ = ['SSync_2_MTASSync']
