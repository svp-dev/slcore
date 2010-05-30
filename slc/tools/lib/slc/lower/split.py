import copy
from ..visitors import DefaultVisitor, flatten
from ..ast import *

class ExampleOracle(object):

    mapping = {'cmta':(0,'fmta'), 'cseq':(1,'fseq')}
    def flavors_for_create(self, cr):
        return ['cmta', 'cseq']

    def flavors_for_fun(self, fd):
        return ['fmta', 'fseq']

    def flavored_funsym(self, name, flavor):
        return "__slF%s_%s" % (flavor, name)

    def funptr_for_lowcreate(self, lc):

        index, f_flavor = self.mapping[lc.flavor]

        if lc.funtype == lc.FUN_ID:
            return self.flavored_funsym(lc.fun, f_flavor)
        else:
            return "((void**)%s)[%d]" % (lc.fun, index)
        

class SplitCreates(DefaultVisitor):

    def __init__(self, oracle = ExampleOracle(), *args, **kwargs):
        super(SplitCreates, self).__init__(*args, **kwargs)
        self.oracle = oracle

    def visit_lowcreate(self, lc):
        """
        Here we multiply a create by duplicating the code.
       
        """
        
        flavors = self.oracle.flavors_for_create(lc)

        flavors.reverse()
        newbl = []
        next = None
        for f in flavors:
            thisalt = []

            newnext = "__slC%s_%s" % (f, lc.label)
            thisalt.append(flatten(lc.loc, " %s: (void)0;" % newnext))

            newlc = copy.deepcopy(lc)
            newlc.target_next = next
            newlc.flavor = f

            funexpr = self.oracle.funptr_for_lowcreate(newlc)

            if newlc.funtype == newlc.FUN_PTR:
                newfun = "__slC%s_fptr_%s" % (f, newlc.label)
                thisalt.append(flatten(newlc.loc, 
                                       "void * const %s = %s;"
                                       % (newfun, funexpr)))
                newlc.fun = newfun
            else:
                newlc.fun = funexpr
            
            thisalt.append(Flavor(f, items = [newlc]))
            thisalt.append(flatten(lc.loc_end, " goto %s;" % lc.target_resolved))

            newbl = thisalt + newbl
            next = newnext
            
        return newbl


class SplitFuns(DefaultVisitor):

    def __init__(self, oracle = ExampleOracle(), *args, **kwargs):
        super(SplitFuns, self).__init__(*args, **kwargs)
        self.oracle = oracle


    def visit_fundecl(self, fd):

        flavors = self.oracle.flavors_for_fun(fd)

        newbl = []
        for f in flavors:
            newfd = FunDecl(name = self.oracle.flavored_funsym(fd.name, f), 
                            parms = copy.deepcopy(fd.parms),
                            loc = fd.loc)
            newbl.append(Flavor(f, items = [ newfd ]))
            newbl.append(flatten(fd.loc, ';'))
        newbl.append(flatten(fd.loc, "extern void * restrict const %s" % fd.name))
        
        return newbl

    def visit_fundef(self, fd):
        
        name = fd.name

        newbl = self.dispatch(fd, seen_as = FunDecl)
        newbl.append(flatten(fd.loc, ';'))

        flavors = self.oracle.flavors_for_fun(fd)


        for f in flavors:
            newfd = copy.deepcopy(fd)
            newfd.name = self.oracle.flavored_funsym(name, f)
            newbl.append(Flavor(f, items = [ newfd ]))



        if fd.static:
            qual = "static"
        else:
            qual = ""

        nflavors = len(flavors)
        lfuncs = ', '.join((self.oracle.flavored_funsym(name, f) for f in flavors))

        # newbl.append(flatten(fd.loc, 
        #                      '%(qual1)s void * __slD_%(name)s[%(nflavors)d] = '
        #                      '{ %(lfuncs)s };'
        #                      '%(qual2)s void *%(name)s '
        #                      '__attribute__((__alias__("__slD_%(name)s")));'
        #                      % locals()))
        newbl.append(flatten(fd.loc, 
                             'static void * const __slD_%(name)s[%(nflavors)d] = '
                             '{ %(lfuncs)s };'
                             '%(qual)s void * restrict const %(name)s = &__slD_%(name)s;'
                             % locals()))


        return newbl
                

__all__ = ['SplitFuns', 'SplitCreates']


