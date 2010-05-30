import sys
from ast import *
from front import opts

#### Common services ####

def flatten(loc, opaquetext):
      o = Opaque(opaquetext)
      if loc is not None and opts.resolved.synclines:
            o = Opaque('\n%s\n' % loc.ppline()) + o
      else:
            o = Opaque('\n') + o
      return o

#### Base visitor and dispatch ####

class BaseVisitor(object):
      def __init__(self, dispatcher = None):
            if dispatcher is not None:
                  self.__dict__ = dispatcher.__dict__
            self.dispatcher = dispatcher

      def dispatch(self, item, seen_as = None, *args, **kwargs):
            if seen_as is None:
                  seen_as = item.__class__

            if self.dispatcher is not None:
                  return self.dispatcher.dispatch(self, item, seen_as, *args, **kwargs)
            else:
                  meth = getattr(self, 'visit_%s' % seen_as.__name__.lower())
                  return meth(item, *args, **kwargs)

      def visit_flavor(self, flavor):
            if self.dispatcher is not None:
                  self.dispatcher.start_flavor(flavor.flavor)

            #print "before visit fl: %r" % flavor
            flavor = self.dispatch(flavor, seen_as = Block)
            #print "after visit fl: %r" % flavor

            if self.dispatcher is not None:
                  self.dispatcher.end_flavor(flavor.flavor)
            
            return flavor


class Dispatcher(object):
      
      def __init__(self, flavored_visitors):
            self.flavored_visitors = flavored_visitors
            for k, v in self.flavored_visitors.items():
                  self.__dict__.update(v.__dict__)
                  v.__dict__ = self.__dict__
                  #print "REG V: %x, d = %x" % (id(v), id(v.__dict__))
            self.dispatcher = self
                  
            #print self.__dict__
            self.current_flavors = []

      def start_flavor(self, f):
            self.current_flavors.insert(0, f)

      def end_flavor(self, f):
            assert self.current_flavors[0] == f
            self.current_flavors.pop(0)

      def dispatch(self, base_visitor, item, seen_as, *args, **kwargs):
            # here we look up the method in the 
            # flavored visitors first, and fall back
            # to the base visitor if that fails.
            
            methname = 'visit_%s' % seen_as.__name__.lower()
            for f in self.current_flavors:
                  v = self.flavored_visitors.get(f, None)
                  if v is not None:
                        if methname in v.__class__.__dict__:
                              m = getattr(v, methname)
                              return m(item, *args, **kwargs)
            # here, no visitor is found
            m = getattr(base_visitor, methname)
            return m(item, *args, **kwargs)


#### Default visitor: just visit ####

class DefaultVisitor(BaseVisitor):
      def visit_program(self, program):
            return self.dispatch(program, seen_as = Block)

      def visit_opaque(self, opaque):
            return opaque

      def visit_block(self, block):
            newbl = Block()
            # print "visiting block %x: %r" % (id(block), block)
            for item in block:
                  # print "in block %x: item %x: %r" % (id(block), id(item), item)
                  newbl += item.accept(self)
            block.reset(newbl)
            #print "return %x: %r" % (id(block), block)
            return block

      def visit_scope(self, scope):
            scope.decls = scope.decls.accept(self)
            return self.dispatch(scope, seen_as = Block)

      def visit_fundecl(self, fundecl):
            p = FunDecl()
            for parm in fundecl.parms:
                  p += parm.accept(self)
            fundecl.reset(p)
            return fundecl

      def visit_fundef(self, fundef):
            self.dispatch(fundef, seen_as = FunDecl)
            fundef.body.accept(self)
            return fundef

      def visit_getp(self, getp):
            return getp

      def visit_geta(self, geta):
            return geta

      def visit_setp(self, setp):
            setp.rhs.accept(self)
            return setp

      def visit_seta(self, seta):
            seta.rhs.accept(self)
            return seta

      def visit_create(self, create):
            create.place.accept(self)
            create.start.accept(self)
            create.step.accept(self)
            create.limit.accept(self)
            create.block.accept(self)
            if create.funtype == create.FUN_OPAQUE:
                  create.fun.accept(self)
            a = []
            for arg in create.args:
                  new = arg.accept(self)
                  if isinstance(new, list):
                        a += new
                  elif new is not None:
                        a.append(new)
            create.args = a

            create.body.accept(self)
            if create.result_lvalue is not None:
                  create.result_lvalue.accept(self)

            return create

      def visit_funparm(self, parm):
            parm.ctype.accept(self)
            return parm

      def visit_createarg(self, arg):
            arg.ctype.accept(self)
            if arg.init is not None:
                arg.init.accept(self)
            return arg

      def visit_endthread(self, et):
            return et

      def visit_break(self, br):
            return br

      def visit_indexdecl(self, idecl):
            return idecl

      def visit_lowcreate(self, lc):
          if lc.lowfun is not None:
                lc.lowfun.accept(self)
          lc.body.accept(self)
          return lc

      def visit_cvardecl(self, cd):
            cd.ctype.accept(self)
            cd.init.accept(self)
            return cd

      def visit_ctypeuse(self, ct):
            return ct

      def visit_cvaruse(self, cu):
            return cu

      def visit_cvarset(self, vs):
            vs.rhs.accept(self)
            return vs

      def visit_clabel(self, cl):
            return cl

      def visit_cgoto(self, cg):
            return cg

      def visit_ccast(self, cc):
            cc.ctype.accept(self)
            cc.expr.accept(self)
            return cc

      def visit_cignore(self, ci):
            return ci

      def visit_cindex(self, ci):
            ci.expr.accept(self)
            ci.index.accept(self)
            return ci

      def visit_ctypehead(self, cth):
            return cth

      def visit_ctype(self, ct):
            return self.dispatch(ct, seen_as = Block)

      def visit_ctypedecl(self, ctd):
            ctd.ctype.accept(self)
            return ctd

#### Scoped visitor: keep track of scopes ####

class ScopedVisitor(DefaultVisitor):

      def __init__(self, *args, **kwargs):
            super(ScopedVisitor, self).__init__(*args, **kwargs)
            self.cur_scope = None

      def visit_scope(self, sc):
            old = self.cur_scope
            self.cur_scope = sc
            # print "IN SCOPE (v = %x, d = %x, sc = %x, %r)" % (id(self), id(self.__dict__), id(sc), dir(sc))
            sc = super(ScopedVisitor, self).visit_scope(sc)
            self.cur_scope = old
            return sc

#### Printer visitor: reprint SL code ####


class PrintVisitor(DefaultVisitor):
    def __init__(self, stream = None, *args, **kwargs):
        super(PrintVisitor, self).__init__(*args, **kwargs)

        if stream is None:
            stream = sys.stdout
            
        self.__out = stream
            
    def visit_opaque(self, opaque):
        if opaque.text is not None:
            if opaque.text.startswith('#'):
                self.__out.write('\n')
            self.__out.write(opaque.text)
        return opaque
            
    def visit_funparm(self, parm):
        self.__out.write(' sl_%s(' % parm.type)
        parm.ctype.accept(self)
        self.__out.write(', %s)' % parm.name)
        return parm

    def visit_funheader(self, fun, htype):
        self.__out.write(" sl_%s(%s, (%s)" 
                         % (htype, fun.name, ','.join((repr(x) for x in fun.extras))))
        for parm in fun.parms:
            self.__out.write(', ')
            parm.accept(self)
        self.__out.write(')')
        return fun

    def visit_fundecl(self, fundecl):
        return self.visit_funheader(fundecl, 'decl')
    
    def visit_fundef(self, fundef):
        self.visit_funheader(fundef, 'def')
        fundef.body.accept(self)
        self.__out.write(' sl_enddef ')
        return fundef

    def visit_flavor(self, f):
          self.__out.write('\n/* BEGIN FLAVOR: %s */\n' % f.flavor)
          DefaultVisitor.visit_flavor(self, f)
          self.__out.write('\n/* END FLAVOR: %s */\n' % f.flavor)
          return f

    def visit_scope(self, scope):
          self.__out.write('{')
          scope.decls.accept(self)
          self.dispatch(scope, seen_as = Block)
          self.__out.write('}')
          return scope

    def visit_getp(self, vu):
        self.__out.write(' sl_getp(%s)' % vu.name)
        return vu

    def visit_geta(self, vu):
        self.__out.write(' sl_geta(%s)' % vu.name)
        return vu

    def visit_setp(self, vu):
        self.__out.write(' sl_setp(%s, ' % vu.name)
        vu.rhs.accept(self)
        self.__out.write(')')
        return vu

    def visit_seta(self, vu):
        self.__out.write(' sl_seta(%s, ' % vu.name)
        vu.rhs.accept(self)
        self.__out.write(')')
        return vu

    def visit_ctypehead(self, cth):
          self.__out.write(' sl_CTH ')
          return cth

    def visit_ctype(self, ct):
          self.__out.write(' sl_ctype(')
          self.dispatch(ct, seen_as = Block)
          self.__out.write(')')
          return ct

    def visit_createarg(self, arg):
        self.__out.write(' sl_%s(' % arg.type)
        arg.ctype.accept(self)
        self.__out.write(', %s' % arg.name)
        if arg.init is not None:
            self.__out.write(',')
            arg.init.accept(self)
        self.__out.write(')')
        return arg

    def visit_break(self, br):
        self.__out.write(' sl_break ')
        return br

    def visit_indexdecl(self, idecl):
          self.__out.write(' sl_index(%s)' % idecl.indexname)
          return idecl

    def visit_endthread(self, et):
        self.__out.write(' sl_end_thread ')
        return et

    def visit_create(self, c):
        self.__out.write(' sl_create(')

        for b in (c.place, c.start, c.step, c.limit, c.block):
            self.__out.write(',')
            b.accept(self)

        self.__out.write(', ,')

        if c.funtype == c.FUN_ID:
            self.__out.write(c.fun)
        elif c.funtype == c.FUN_VAR:
              self.__out.write('sl_cvaruse(%s)' % c.fun.name)
        else:
            c.fun.accept(self)

        for arg in c.args:
            self.__out.write(',')
            arg.accept(self)
        self.__out.write(')')

        c.body.accept(self)
          
        if c.sync_type == 'release':
            self.__out.write(' sl_release()')
        else:
            self.__out.write(' sl_sync(')
            if c.result_lvalue is not None:
                c.result_lvalue.accept(self)
            self.__out.write(')')

        return c

    def visit_lowcreate(self, lc):
        self.__out.write(' sl_lowcreate(%s' % lc.label)
        if lc.lowfun is not None:
              self.__out.write(', fun: ')
              lc.lowfun.accept(self)
        if lc.target_next is not None:
              self.__out.write(', target_next: %s@0x%x' % (lc.target_next.name, id(lc.target_next)))
              
        self.__out.write(') ')
        lc.body.accept(self)
        self.__out.write(' sl_lowcreate_end(%s) ' % lc.label)
        return lc

    def visit_ctypedecl(self, ctd):
          self.__out.write(' sl_ctypedecl(')
          ctd.ctype.accept(self)
          self.__out.write(', %s) /* 0x%x */\n' % (ctd.name, id(ctd)))
          return ctd

    def visit_cvardecl(self, vd):
          self.__out.write(' sl_cvardecl(')
          vd.ctype.accept(self)
          self.__out.write(', %s' % vd.name)
          if len(vd.init):
                self.__out.write(', ')
                vd.init.accept(self)
          self.__out.write(') /* 0x%x */\n' % id(vd))
          return vd

    def visit_cvaruse(self, vu):
          self.__out.write(' sl_cvaruse(%s /* 0x%x */)' % (vu.decl.name, id(vu.decl)))
          return vu

    def visit_ctypeuse(self, vu):
          self.__out.write(' sl_ctypeuse(%s /* 0x%x */)' % (vu.tdecl.name, id(vu.tdecl)))
          return vu
    
    def visit_cvarset(self, vs):
          self.__out.write(' sl_cvarset(%s /* 0x%x */, ' % (vs.decl.name, id(vs.decl)))
          vs.rhs.accept(self)
          self.__out.write(')')
          return vs

    def visit_clabel(self, cl):
          self.__out.write(' sl_clabel(%s /* 0x%x */)' % (cl.name, id(cl)))
          return cl

    def visit_cgoto(self, cg):
          self.__out.write(' sl_cgoto(%s /* 0x%x */)' % (cg.target.name, id(cg.target)))
          return cg


    def visit_ccast(self, cc):
          self.__out.write(' sl_ccast(')
          cc.ctype.accept(self)
          self.__out.write(', ')
          cc.expr.accept(self)
          self.__out.write(')')
          return cc

    def visit_cignore(self, ci):
          self.__out.write('\n#if 0 /* IGNORED TEXT */\n')
          ci.body.accept(self)
          self.__out.write('\n#endif /* END IGNORED TEXT */\n')
          return ci

    def visit_cindex(self, ci):
          self.__out.write('((')
          ci.expr.accept(self)
          self.__out.write(')[')
          ci.index.accept(self)
          self.__out.write('])')
          return ci

__all__ = ['flatten', 'BaseVisitor', 'Dispatcher', 'DefaultVisitor', 'ScopedVisitor', 'PrintVisitor']
