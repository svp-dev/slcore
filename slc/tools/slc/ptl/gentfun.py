from ..visitors import DefaultVisitor, ScopedVisitor, flatten
from ..ast import *
from ..msg import warn

class TFun_2_ptlTFun(DefaultVisitor):

    def __init__(self, *args, **kwargs):
        super(TFun_2_ptlTFun, self).__init__(*args, **kwargs)
        self.__shlist = None
        self.__gllist = None

    def visit_getp(self, getp):
        if getp.name in self.__shlist:
            format = "(__slP_%s)"
        else:
            format = " __slP_%s "
        return flatten(getp.loc, format % getp.name)

    def visit_setp(self, getp):
        b = getp.rhs.accept(self)
        if getp.name in self.__shlist:
            format = "(__slP_%s) = "
        else:
            format = " __slP_%s = "
        return [flatten(getp.loc, format % getp.name), b]

    def visit_funparm(self, parm):
        if self.__shlist or self.__gllist:
            self.__buffer += ', '
        if parm.type.startswith("sh"):
            self.__shlist.append(parm.name)
            self.__buffer += (Opaque('uTC::shared<') + parm.ctype + 
                              '>& __slP_%s' % parm.name)
        else:
            self.__gllist.append(parm.name)
            self.__buffer += (Opaque('uTC::global< ') + parm.ctype + 
                              '>& __slP_%s' % parm.name)
        return parm

    def visit_fundecl(self, fundecl, infundef = False):
        old_shlist = self.__shlist
        old_gllist = self.__gllist
        self.__shlist = []
        self.__gllist = []
        qual = ""
        iattr = ""
        if fundecl.extras.get_attr('static', None) is not None:
            qual = "static"
        elif infundef:
            iattr = " __attribute__((unused))"
        else:
            qual = "extern"
        self.__buffer = flatten(fundecl.loc, 
                                " %s void %s(" 
                                % (qual, fundecl.name))
        for parm in fundecl.parms:
            parm.accept(self)
        self.__buffer += ')'
        ret = self.__buffer
        self.__buffer = None
        if not infundef:
            self.__shlist = old_shlist
            self.__gllist = old_gllist
        return ret
 
    def visit_fundeclptr(self, fundecl):
        old_shlist = self.__shlist
        old_gllist = self.__gllist
        self.__shlist = []
        self.__gllist = []
        if fundecl.extras.get_attr('typedef', None) is not None:
            qual = "typedef"
        elif fundecl.extras.get_attr('static', None) is not None:
            qual = "static"
        else:
            qual = ''
        self.__buffer = flatten(fundecl.loc, 
                                " %s void (*%s)( " 
                                % (qual, fundecl.name))
        for parm in fundecl.parms:
            parm.accept(self)
        self.__buffer += ')'
        ret = self.__buffer
        self.__buffer = None
        self.__shlist = old_shlist
        self.__gllist = old_gllist
        return ret

    def visit_fundef(self, fundef):
        newitems = self.dispatch(fundef, seen_as = FunDecl, infundef = True)
        newitems += flatten(fundef.loc, "{")
        newitems += fundef.body.accept(self)
        newitems += flatten(fundef.loc_end, " }")
        self.__shlist = self.__gllist = None
        return newitems

    def visit_indexdecl(self, idecl):
        return flatten(idecl.loc, 
                       " uTC::index %s" 
                       % idecl.indexname) 

    def visit_break(self, br):
        return flatten(br.loc, " uTC::Break() ")

    def visit_endthread(self, et):
        return flatten(et.loc, " return ")


__all__ = ['TFun_2_ptlTFun']
