from ..visitors import DefaultVisitor
from ..msg import log, warn
from ..ast import *

class CheckVisitor(DefaultVisitor):

    def err(self, msg, context = None):
        log('error: %s' % msg, context)
        self.has_errors = True

    def __init__(self, *args, **kwargs):
        super(CheckVisitor, self).__init__(*args, **kwargs)

        self.save_stack = []

        self.has_errors = False

        self.cur_create = None
        self.cur_scope = None
        self.cur_fun = None
        self.in_parm_list = False
        self.in_arg_list = False

    def enter(self, **kwargs):       
        save = {}
        for k in kwargs:
            save[k] = getattr(self, k)
        self.save_stack.insert(0, save)
        self.__dict__.update(kwargs)

    def leave(self):
        assert len(self.save_stack) > 0
        save = self.save_stack.pop(0)
        self.__dict__.update(save)

    def visit_funparm(self, parm):
        if not self.in_parm_list:
            self.err("thread parameter declaration outside of parameter list", parm)
            return parm

        assert self.cur_fun is not None

        pdic = self.cur_fun.parm_dic
        if parm.name in pdic:
            self.err("redefinition of parameter '%s'" % parm.name, parm)
            self.err("  previous definition of '%s' was here" % parm.name, pdic[parm.name])
            return parm

        # cross-link for later
        parm.fun = self.cur_fun
        pdic[parm.name] = parm

        return parm
        

    def visit_fundecl(self, fd):
        fd.parm_dic = {}

        self.enter(cur_fun = fd, in_parm_list = True)
        for p in fd.parms:
            p.accept(self)
        self.leave()

    def visit_fundef(self, fd):
        if self.cur_fun is not None:
            self.err("cannot nest thread function definition", fd)
            self.err("  in function definition starting here", self.cur_fun)
            return fd
        
        self.dispatch(fd, seen_as = FunDecl)
        
        self.enter(cur_fun = fd)
        fd.body.accept(self)
        self.leave()

        # some checks on parameters
        for p in fd.parms:
            if not p.seen_set and not p.seen_get:
                warn("unused thread parameter '%s'" % p.name, fd)
            else:
                if not p.seen_set and p.type.startswith('sh'):
                    warn("shared parameter '%s' is read but not written to" % p.name, fd)
                if not p.seen_get and p.type.startswith('sh'):
                    warn("shared parameter '%s' is written but not read from" % p.name, fd)
                        
    def visit_block(self, block):
        self.enter(in_parm_list = False)
        for item in block:
            item.accept(self)
        self.leave()

    def visit_break(self, br):
        if self.cur_fun is None or self.cur_scope is None:
            self.err("'break' outside of thread function body", br)
        if self.cur_create is not None:
            self.err("'break' inside of 'create' construct", br)
        br.fun = self.cur_fun
        return br

    def visit_endthread(self, et):
        if self.cur_fun is None or self.cur_scope is None:
            self.err("'end thread' outside of thread function body", et)
        if self.cur_create is not None:
            self.err("'end thread' inside of 'create' construct", et)
        et.fun = self.cur_fun
        return et

    def visit_indexdecl(self, idecl):
        if self.cur_fun is None or self.cur_scope is None:
            self.err("index declaration outside of thread function body", idecl)
        idecl.fun = self.cur_fun
        idecl.scope = self.cur_scope
        return idecl

    def visit_scope(self, scope):
        scope.arg_dic = {}
        scope.local_args = []
        scope.hot_args = set()

        if self.cur_scope is not None:
            scope.hot_args.update(self.cur_scope.hot_args)
            scope.arg_dic.update(self.cur_scope.arg_dic)

        self.enter(cur_scope = scope)
        self.dispatch(scope, seen_as = Block)
        self.leave()

        return scope

    def visit_getsetp(self, p):
        if self.cur_fun is None:
            self.err("access to thread parameter outside of thread function definition", p)
            return p

        if p.name not in self.cur_fun.parm_dic:
            self.err("thread parameter '%s' undeclared (first use; reported only once)" % p.name, p)
            self.cur_fun.parm_dic[p.name] = None
            return p

        p.decl = self.cur_fun.parm_dic[p.name]
        p.fun = self.cur_fun

        if hasattr(p, 'rhs'):
            if self.cur_fun.parm_dic[p.name].type.startswith('gl'):
                self.err("invalid write to global thread parameter '%s'" % p.name, p)
            p.rhs.accept(self)
            p.decl.seen_set = True
        else:
            p.decl.seen_get = True

        return p

    visit_getp = visit_getsetp
    visit_setp = visit_getsetp

    def visit_getseta(self, p):
        if self.cur_scope is None:
            self.err("access to create argument outside of C block", p)
            return p

        adic = self.cur_scope.arg_dic
        if p.name not in adic:
            self.err("create argument '%s' undeclared (first use in this scope; reported only once)" % p.name, p)
            a = CreateArg(loc = p.loc, name = p.name)
            a.scope = self.cur_scope
            adic[p.name] = a
            self.cur_scope.local_args.append(p.name)
            return p

        p.decl = adic[p.name]
        p.scope = p.decl.scope

        if hasattr(p, 'rhs'):
            if not p.name in p.scope.hot_args:
                self.err("write to create argument '%s' outside of create construct" % p.name, p)
                self.err("  after create construct starting here", p.decl.create)
                self.err("  after create construct ending here", p.decl.create.loc_end)
            p.rhs.accept(self)
            p.decl.seen_set = True
        else:
            if p.name in p.scope.hot_args:
                self.err("read from create argument '%s' inside create construct" % p.name, p)
                self.err("  in create construct starting here", p.decl.create)
                self.err("  in create construct ending here", p.decl.create.loc_end)
            else:
                # FIXME: check that geta does not occur after detach in a continuation create
                pass
            p.decl.seen_get = True

        return p

    visit_geta = visit_getseta
    visit_seta = visit_getseta

    def visit_createarg(self, arg):
        if not self.in_arg_list:
            self.err("create argument declaration outside of argument list", parm)
            return arg

        assert self.cur_create is not None
        assert self.cur_scope is not None

        scope = self.cur_scope

        sadic = scope.arg_dic
        sal = scope.local_args
        if arg.name in sal:
            self.err("redefinition of create argument '%s'" % arg.name, arg)
            self.err("previous definition of '%s' was here" % arg.name, sadic[arg.name])
            return arg
        
        # cross-link for later
        arg.create = self.cur_create
        arg.scope = scope
        scope.hot_args.add(arg.name)
        sal.append(arg.name)
        sadic[arg.name] = arg
        self.cur_create.arg_dic[arg.name] = arg

        return arg

    def visit_create(self, c):
        if self.cur_scope is None:
            self.err("'create' outside of C block", c)
            return c

        c.scope = self.cur_scope

        if c.label in c.scope.creates:
            self.err("'create' with label %s already in scope" % c.label)
            self.err("previous definition of %s was here" % c.label, c.scope.creates[c.label])
            return c
        c.scope.creates[c.label] = c

        for b in (c.place, c.start, c.step, c.limit, c.block):
            b.accept(self)

        if c.funtype == c.FUN_OPAQUE:
            c.fun.accept(self)

        # preprocess initializers before
        # defining arguments, so that they can reference upper
        # level declarations
        for a in c.args:
            if a.init is not None:
                a.init.accept(self)
                a.seen_set = True

        c.arg_dic = {}

        self.enter(cur_create = c, in_arg_list = True)
        for a in c.args:
            a.accept(self)
        self.leave()

        self.enter(cur_create = c)
        c.body.accept(self)
        self.leave()
        
        for a in c.args:
            c.scope.hot_args.discard(a.name)
            if not a.seen_set:
                warn("create argument '%s' not initialized" % a.name, c.loc_end)
                warn("in create construct starting here", c.loc) 
            

        if c.result_lvalue is not None:
            c.result_lvalue.accept(self)

        return c

__all__ = ['CheckVisitor']

