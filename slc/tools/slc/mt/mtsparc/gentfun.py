from regdefs import regmagic
from ..common.gentfun import TFun_2_MTFun
from ...msg import die

class TFun_2_MTSTFun(TFun_2_MTFun):

    def __init__(self, newisa = False, *args, **kwargs):
        self.newisa = newisa
        super(TFun_2_MTSTFun, self).__init__(regmagic, *args, **kwargs)

    def visit_indexdecl(self, idecl):
        if idecl.extraarg and self.newisa:
            die("sl__extra not supported for this target", idecl)
        return super(TFun_2_MTSTFun, self).visit_indexdecl(idecl)

__all__ = ['TFun_2_MTSTFun']
