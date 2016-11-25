from regdefs import regmagic
from ..common.gentfun import TFun_2_MTFun
from ...visitors import flatten

class TFun_2_L2MTTFun(TFun_2_MTFun):

    def __init__(self, *args, **kwargs):
        self.unsigned_ix = True
        super(TFun_2_L2MTTFun, self).__init__(regmagic, *args, **kwargs)

    def visit_indexdecl(self, idecl):
        b = []
        b.append(super(TFun_2_L2MTTFun, self).visit_indexdecl(idecl))
        b.append(flatten(idecl.loc,
                         '; register union { struct { unsigned y : 16; unsigned x : 16; }; unsigned __slI; } threadIdx; threadIdx.__slI = __slI;'
                         ' register union { struct { unsigned y : 16; unsigned x : 16; }; unsigned __slB; } blockIdx; blockIdx.__slB = __slB;'
                         ' register union { struct { unsigned y : 16; unsigned x : 16; }; unsigned __slS; } blockDim; blockDim.__slS = __slBS;'
                         ' register union { struct { unsigned y : 16; unsigned x : 16; }; unsigned __slS; } gridDim; gridDim.__slS = __slGS'))
        return b
        
    def visit_indexdef(self, fundef):
        return flatten(fundef.loc,
                       ' register unsigned long __slGS __attribute__((unused));'
                       ' __asm__ ("f_get_gridsize %0" : "=r"(__slGS));'
                       ' register unsigned long __slBS __attribute__((unused));'
                       ' __asm__ ("f_get_blocksize %0" : "=r"(__slBS));'
                       ' register unsigned long __slB __attribute__((unused));'
                       ' __asm__ ("f_get_blockindex %0" : "=r"(__slB));')
        
__all__ = ['TFun_2_L2MTTFun']
