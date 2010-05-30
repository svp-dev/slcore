from ..visitors import DefaultVisitor

class RenameCVars(DefaultVisitor):
    def __init__(self, *args, **kwargs):
        super(RenameCVars, self).__init__(*args, **kwargs)
        self.counter = 0

    def visit_cvardecl(self, cv):
        cv.init.accept(self)
        cv.name = '__Scv$' + cv.name + "$%d" % self.counter
        self.counter += 1
        return cv

class RenameCLabels(DefaultVisitor):
      
    def __init__(self, *args, **kwargs):
        super(RenameCLabels, self).__init__(*args, **kwargs)
        self.counter = 0

    def visit_clabel(self, cv):
        cv.name = '__Scl$' + cv.name + "$%d" % self.counter
        self.counter += 1
        return cv


__all__ = ['RenameCVars', 'RenameCLabels']

