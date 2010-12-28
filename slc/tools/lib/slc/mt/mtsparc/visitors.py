from ...visitors import DefaultVisitor, ScopedVisitor, flatten

class Create_2_MTSCreate(ScopedVisitor):
    pass

class TFun_2_MTSTFun(DefaultVisitor):
    pass


__all__ = ['Create_2_MTSCreate', 'TFun_2_MTSTFun']
