from regdefs import regmagic
from ..common.gentfun import TFun_2_MTFun
from ...msg import die

class TFun_2_MTATFun(TFun_2_MTFun):

    def __init__(self, *args, **kwargs):
        super(TFun_2_MTATFun, self).__init__(regmagic, *args, **kwargs)

    def visit_indexdecl(self, idecl):
        if idecl.extraarg:
            die("sl__extra not supported for this target", idecl)
        return super(TFun_2_MTATFun, self).visit_indexdecl(idecl)

__all__ = ['TFun_2_MTATFun']
