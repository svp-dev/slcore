from regdefs import regmagic
from ..common.gentfun import TFun_2_MTFun

class TFun_2_MTSTFun(TFun_2_MTFun):

    def __init__(self, *args, **kwargs):
        super(TFun_2_MTSTFun, self).__init__(regmagic, *args, **kwargs)

__all__ = ['TFun_2_MTSTFun']
