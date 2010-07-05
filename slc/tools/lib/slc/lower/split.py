import copy
from ..visitors import DefaultVisitor, ScopedVisitor, flatten
from ..ast import *
from ..msg import die

class ExampleOracle(object):

    mapping = {'cmta':(0,'fmta'), 'cseq':(1,'fseq')}

    def flavors_for_create(self, cr):
        n = cr.extras.get_attr('naked', None)
        if n is not None:
            if n.flavor == 'native':
                return ['cmta']
            elif n.flavor == 'seq':
                return ['cseq']
            else:
                die("unsupported naked specifier: %s" % n.flavor, cr)
        
        if cr.extras.get_attr('exclusive', None):
            return ['cmta']

        return ['cmta', 'cseq']

    def flavors_for_fun(self, fd):
        n = fd.extras.get_attr('naked', None)
        if n is not None:
            if n.flavor == 'native':
                return (True, ['fmta'])
            elif n.flavor == 'seq':
                return (True, ['fseq'])
            else:
                die("unsupported naked specifier: %s" % n.flavor, fn)
                
        return (False, ['fmta', 'fseq'])

    def flavored_funsym(self, name, flavor):
        return "__slF%s_%s" % (flavor, name)

    def lowfun_for_create(self, cr, flavor):

        if cr.extras.get_attr('naked', None) is not None:
            if cr.funtype == cr.FUN_ID:
                return cr.fun
            else:
                assert cr.funtype == cr.FUN_VAR
                return CVarUse(decl = cr.fun)

        ix, f_flavor = self.mapping[flavor]

        if cr.funtype == cr.FUN_ID:
            d = Opaque(self.flavored_funsym(cr.fun, f_flavor))
            #print "LF2C_id(%s): %r -> %r" % (flavor, cr.fun, d)
            return d
        else:
            assert cr.funtype == cr.FUN_VAR
            #print "LF2C_var(%s): %r" % (flavor, cr.fun)
            return CIndex(expr = CCast(ctype = 'void**', 
                                       expr = CVarUse(decl = cr.fun)),
                          index = Opaque("%d" % ix))
        

class SplitCreates(ScopedVisitor):

    def __init__(self, oracle = ExampleOracle(), *args, **kwargs):
        super(SplitCreates, self).__init__(*args, **kwargs)
        self.oracle = oracle

    def visit_lowcreate(self, lc):
        """
        Here we multiply a create by duplicating the code.
       
        """

        # first split recursively
        lc.body.accept(self)
        
        # retrieve the corresponding create
        cr = self.cur_scope.creates[lc.label]

        # # t_main is a special case
        # if cr.funtype == cr.FUN_ID and cr.fun == "t_main":
        #     # append to the end without replacing, so that
        #     # the user can overload
        #     cr.extras += Attr(name = 'naked', payload = {'flavor':'native'})


        # we need to remember declarations to avoid
        # deep copying of cvaruses/cvarsets
        memo = {}
        for s in self.scope_stack:
            memo[id(s)] = s
        for d in self.cur_scope.decls:
            memo[id(d)] = d

        flavors = self.oracle.flavors_for_create(cr)

        flavors.reverse()
        newbl = []

        next = None
        #import sys, pprint
        for f in flavors:
            newmemo = copy.copy(memo)
            #pprint.pprint(lc, stream=sys.stderr)
            newlc = copy.deepcopy(lc, newmemo)
            newlc.target_next = next
            newlc.flavor = f

            newlc.lowfun = Block(items = self.oracle.lowfun_for_create(cr, f))
            #print "IN SPLIT, gen %x, %r" % (id(newlc), newlc.lowfun)
            
            thisalt = []
            next = CLabel(loc = cr.loc_end, name = "Cn$%s$%s" % (f, lc.label))
            thisalt.append(next)
            thisalt.append(Flavor(f, items = newlc))
            thisalt.append(CGoto(loc = cr.loc_end, target = cr.target_resolved) + ';')

            newbl = thisalt + newbl

        newbl.insert(0, CGoto(loc = cr.loc, target = next) + ';')
            
        return newbl

class SplitFuns(DefaultVisitor):

    def __init__(self, oracle = ExampleOracle(), *args, **kwargs):
        super(SplitFuns, self).__init__(*args, **kwargs)
        self.oracle = oracle


    def visit_fundecl(self, fd):

        # # t_main is a special case
        # if fd.name == "t_main":
        #     # append to the end without replacing, so that
        #     # the user can overload
        #     fd.extras += Attr(name = 'naked', payload = {'flavor':'native'})

        (naked, flavors) = self.oracle.flavors_for_fun(fd)

        if naked:
            return Flavor(flavors[0], items = fd)

        # general case: split
        newbl = []
        for f in flavors:
            newfd = FunDecl(name = self.oracle.flavored_funsym(fd.name, f), 
                            parms = copy.deepcopy(fd.parms),
                            extras = copy.deepcopy(fd.extras),
                            loc = fd.loc)
            newbl.append(Flavor(f, items = [ newfd ]))
            newbl.append(flatten(fd.loc, ';'))

        if fd.extras.get_attr('static', None) is not None:
            qual = "static"
        else:
            qual = "extern"

        newbl.append(flatten(fd.loc, " %s void * restrict const %s[2]" % (qual, fd.name)))
        
        return newbl

    def visit_fundeclptr(self, fd):
        
        (naked, flavors) = self.oracle.flavors_for_fun(fd)
        if naked:
            return Flavor(flavors[0], items = fd)

        if fd.extras.get_attr('typedef', None) is not None:
            qual = 'typedef'
        elif fd.extras.get_attr('static', None) is not None:
            qual = 'static'
        else:
            qual = ''

        return flatten(fd.loc, " %s void * restrict const (*%s)[2]" % (qual, fd.name))

    def visit_fundef(self, fd):

        fd.body.accept(self)

        # # t_main is a special case
        # if fd.name == "t_main":
        #     # append to the end without replacing, so that
        #     # the user can overload
        #     fd.extras += Attr(name = 'naked', payload = {'flavor':'native'})
        
        (naked, flavors) = self.oracle.flavors_for_fun(fd)

        if naked:
            return Flavor(flavors[0], items = fd)
        
        # general case: split

        name = fd.name
        newbl = self.dispatch(fd, seen_as = FunDecl)
        newbl.append(flatten(fd.loc, ';'))

        for f in flavors:
            newfd = copy.deepcopy(fd)
            newfd.name = self.oracle.flavored_funsym(name, f)
            newbl.append(Flavor(f, items = [ newfd ]))



        if fd.extras.get_attr('static', None) is not None:
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
                             ' %(qual)s void * restrict const %(name)s[%(nflavors)d] = '
                             '{ %(lfuncs)s };'
                             #'%(qual)s void * restrict const %(name)s = &__slD_%(name)s;'
                             % locals()))


        return newbl
                

__all__ = ['SplitFuns', 'SplitCreates']


