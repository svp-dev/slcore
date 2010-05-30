import sys
from ast import *

#### Common services ####

def flatten(loc, opaquetext):
      if loc is not None:
            opaquetext = ('\n%s\n' % loc.ppline()) + opaquetext
      return Opaque(opaquetext)

#### Base visitor and dispatch ####

class BaseVisitor(object):
      def __init__(self, dispatcher = None):
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
            if not create.funIsIdentifier():
                  create.fun.accept(self)
            a = []
            for arg in create.args:
                  new = arg.accept(self)
                  if type(new) == type([]):
                        a += new
                  elif new is not None:
                        a.append(new)
            create.args = a

            create.body.accept(self)
            if create.result_lvalue is not None:
                  create.result_lvalue.accept(self)

            return create

      def visit_funparm(self, parm):
            return parm

      def visit_createarg(self, arg):
            if arg.init is not None:
                arg.init.accept(self)
            return arg

      def visit_endthread(self, et):
            return et

      def visit_break(self, br):
            return br

      def visit_lowcreatearg(self, arg):
          return arg

      def visit_lowcreate(self, lc):
          lc.body.accept(self)
          return lc

#### Printer visitor: reprint SL code ####


class PrinterVisitor(DefaultVisitor):
    def __init__(self, stream = None, *args, **kwargs):
        super(PrinterVisitor, self).__init__(*args, **kwargs)

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
        self.__out.write(' sl_%s(%s, %s)' % (parm.type, parm.ctype, parm.name))
        return parm

    def visit_funheader(self, fun, htype):
        self.__out.write(" sl_%s(%s, void" % (htype, fun.name))
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
          self.__out.write('/* BEGIN FLAVOR: %s */' % f.flavor)
          DefaultVisitor.visit_flavor(self, f)
          self.__out.write('/* END FLAVOR: %s */' % f.flavor)
          return f

    def visit_block(self, block):
        if block.indexname is not None:
            self.__out.write('{ sl_index(%s); ' % block.indexname)
        #print "visiting %x: block %x: %r" % (id(self), id(block), block)
        DefaultVisitor.visit_block(self, block)
        if block.indexname is not None:
            self.__out.write('}')
        return block

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

    def visit_createarg(self, arg):
        self.__out.write(' sl_%s(%s, %s' % (arg.type, arg.ctype, arg.name))
        if arg.init is not None:
            self.__out.write(',')
            arg.init.accept(self)
        self.__out.write(')')
        return arg

    def visit_break(self, br):
        self.__out.write(' sl_break ')
        return br

    def visit_endthread(self, et):
        self.__out.write(' sl_end_thread ')
        return et

    def visit_create(self, c):
        self.__out.write(' sl_create(')

        for b in (c.place, c.start, c.step, c.limit, c.block):
            self.__out.write(',')
            b.accept(self)

        self.__out.write(',/**/,')

        if c.funIsIdentifier():
            self.__out.write(c.fun)
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

    def visit_lowcreatearg(self, arg):
        self.__out.write(' sl_%s(%s, %s' % (arg.type, arg.ctype, arg.name))
        if arg.init is not None:
            self.__out.write(', %s' % arg.init)
        self.__out.write(')')
        return arg

    def visit_lowcreate(self, lc):
        self.__out.write(' /*LOW*/sl_create(')

        for b in (lc.place, lc.start, lc.step, lc.limit, lc.block):
            self.__out.write(', %s' % b)

        self.__out.write(',/**/, %s' % lc.fun)

        for arg in lc.args:
            self.__out.write(',')
            arg.accept(self)
        self.__out.write(')')

        lc.body.accept(self)
          
        if lc.sync_type == 'release':
            self.__out.write(' /*LOW*/sl_release()')
        else:
            self.__out.write(' /*LOW*/sl_sync(%s)' % lc.retval)

        return lc
        

__all__ = ['flatten', 'BaseVisitor', 'DefaultVisitor', 'PrinterVisitor']
