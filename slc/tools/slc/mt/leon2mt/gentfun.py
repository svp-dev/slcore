from regdefs import regmagic
from ..common.gentfun import TFun_2_MTFun
from ...visitors import flatten

class TFun_2_L2MTTFun(TFun_2_MTFun):

    def __init__(self, *args, **kwargs):
        self.unsigned_ix = True
        super(TFun_2_L2MTTFun, self).__init__(regmagic, *args, **kwargs)

    def visit_indexdef(self, fundef):
        return flatten(fundef.loc,
                       ' register union { struct { unsigned y : 16; unsigned x : 16; }; long __slI; } threadIdx __attribute__((unused)); threadIdx.__slI = __slI_;')
        
__all__ = ['TFun_2_L2MTTFun']
