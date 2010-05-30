from loc import splitloc, Loc
import lex
import copy

class Item(object):
    """
    Base class for all AST nodes.
    """

    def __init__(self, loc = None, loc_end = None, *args, **kwargs):
        super(Item, self).__init__(*args, **kwargs)
        self.loc = loc
        self.loc_end = loc_end
        
    @property
    def loc(self): return self._loc

    @property
    def loc_end(self): return self._loc_end

    @loc.setter
    def loc(self, loc):
        if isinstance(loc, str): loc = splitloc(loc)
        assert loc is None or isinstance(loc, Loc)
        self._loc = loc
        
    @loc_end.setter
    def loc_end(self, loc_end):
        if isinstance(loc_end, str): loc_end = splitloc(loc_end)
        assert loc_end is None or isinstance(loc_end, Loc)
        self._loc_end = loc_end

    def clone(self):
        return copy.deepcopy(self)

    def accept(self, v):
        return v.dispatch(self)

    def __repr__(self):
        return '%s@0x%x(%s)' % (self.__class__.__name__, id(self),
                           ', '.join(('%s = %r' % ((k[0] == '_' and k[1:] or k), v) 
                                      for (k, v) in self.__dict__.iteritems()
                                      if v is not None 
                                      and ((len(k)>=1 and k[0] != '_') or
                                           (len(k)>=2 and k[0]=='_' and k[1] != '_'))
                                     ))
                           )


class Opaque(Item):
    """
    Opaque chunk of C text.
    """

    def __init__(self, text = None, *args, **kwargs):
        super(Opaque, self).__init__(*args, **kwargs)
        self.text = text

class Block(Item):
    """
    Sequence of AST nodes, possibly annotated with an index name.
    """

    def __init__(self, items = None, indexname = None, *args, **kwargs):
        super(Block, self).__init__(*args, **kwargs)
        #if isinstance(items, Block):
        #    items = items._items
        if items is None:
            items = []
        self._items = items
        self._indexname = indexname

    def __iadd__(self, item):
        #if isinstance(item, Block):
        #    self._items += item._items
        #el
        if isinstance(item, list):
            self._items += item
        elif item is not None:
            self._items.append(item)
        return self

    def __len__(self):
        return len(self._items)

    def __getitem__(self, index):
        return self._items[index]

    def __iter__(self):
        return self._items.__iter__()

    def reset(self, items):
        if isinstance(items, Block):
            items = items._items
        self._items = items

    @property
    def indexname(self): return self._indexname

    @indexname.setter
    def indexname(self, name):
        if name is not None:
            name = lex.extract_id(self, name)
        self._indexname = name

class Scope(Block):
    """
    A block surrounded by {}.
    """
    def __init__(self, decls = None, *args, **kwargs):
        super(Scope, self).__init__(*args, **kwargs)
        if not isinstance(decls, Block):
            decls = Block(items = decls)
        self.decls = decls

class Flavor(Block):
    """
    Flavor root for a branch of the AST.
    """

    def __init__(self, flavor = None, *args, **kwargs):
        super(Flavor, self).__init__(*args, **kwargs)
        self.flavor = flavor
            
class Program(Block):
    """
    Root node for a program.
    """

    pass


class FunDeclBase(Item):

    def __init__(self, name = None, parms = None, *args, **kwargs):
        super(FunDeclBase, self).__init__(*args, **kwargs)
        self._name = name

        if parms is None: parms = []
        self.parms = parms

    def __len__(self):
        return len(self.parms)
    
    def __iter__(self):
        return self.parms.__iter__()
    
    def __iadd__(self, parm):
        if isinstance(parm, list):
            self.parms += parm
        elif parm is not None:
            self.parms.append(parm)
        return self

    def reset(self, parms):
        if isinstance(parms, FunDecl):
            parms = parms.parms
        self.parms = parms

    @property
    def name(self): return self._name

    @name.setter
    def name(self, name):
        self._name = lex.extract_id(self, name)


class FunDeclPtr(FunDeclBase):
    """
    Function pointer declaration node.

    Used to represent ``sl_decl_fptr``.
    """

    pass

class FunDecl(FunDeclBase):
    """
    Function declaration node.

    Used to represent ``sl_decl``.
    """

    pass

class FunDef(FunDecl):
    """
    Function definition node.

    Used to represent ``sl_def``.
    """

    def __init__(self, body = None, static = False, *args, **kwargs):
        super(FunDef, self).__init__(*args, **kwargs)
        self.static = static
        self.body = body

class ArgParm(Item):
    """
    Base node type for arguments/parameters.
    """
    def __init__(self, name = None, ctype = None, type = None, *args, **kwargs):
        super(ArgParm, self).__init__(*args, **kwargs)
        self._name = name
        self.ctype = ctype
        self.type = type
        self.seen_set = False
        self.seen_get = False

    @property
    def name(self): return self._name

    @name.setter
    def name(self, name):
        self._name = lex.extract_id(self, name)

class FunParm(ArgParm):
    """
    Base node type for function parameters.

    Used to represent ``sl_*parm``.
    """
    pass

class CreateArg(ArgParm):
    """
    Node type for create arguments.

    Used to represent ``sl_*arg``.
    """
    def __init__(self, init = None, *args, **kwargs):
        super(CreateArg, self).__init__(*args, **kwargs)
        self.init = init
        

class VarUse(Item):
    """
    Base node types for uses of arguments and parameters.

    Derived classes implement ``sl_get*``.
    """

    def __init__(self, name = None, *args, **kwargs):
        super(VarUse, self).__init__(*args, **kwargs)
        self._name = name

    @property
    def name(self): return self._name

    @name.setter
    def name(self, name):
        self._name = lex.extract_id(self, name)

class VarSet(VarUse):
    """
    Base node types for writes to arguments and parameters.

    Derived classes implement ``sl_set*``.
    """

    def __init__(self, rhs = None, *args, **kwargs):
        super(VarSet, self).__init__(*args, **kwargs)
        self.rhs = rhs

class GetP(VarUse): 
    pass

class GetA(VarUse): 
    pass

class SetP(VarSet): 
    pass

class SetA(VarSet): 
    pass

class Create(Item):
    """
    Create-sync or Create-detach block.
    """

    def __init__(self, label = None, place = None, 
                 start = None, step = None, limit = None, block = None,
                 sync_type = None, fun = None, body = None,
                 result_lvalue = None, 
                 args = None,
                 *a, **kwargs):
        super(Create, self).__init__(*a, **kwargs)
        self.label = label
        self.place = place
        self.start = start
        self.step = step
        self.limit = limit
        self.block = block
        self.sync_type = sync_type
        self.body = body
        self.result_lvalue = result_lvalue
        self.fun = fun
        if args is None:
            args = []
        self.args = args

    @property
    def label(self): return self._label

    @label.setter
    def label(self, label): 
        if label is not None:
            label = lex.extract_id(self, label)
        self._label = label

    @property
    def fun(self): return self._fun

    @fun.setter
    def fun(self, fun):
        assert isinstance(fun, Block)
        if len(fun) == 1 and isinstance(fun[0], Opaque):
            (t, i) = lex.is_simple_identifier(fun[0].text)
            if t:
                self._fun = i
                return
            self._fun = fun

    def funIsIdentifier(self):
        return isinstance(self.fun, str)

class LowCreateArg(CreateArg):
    """
    Node type for LowCreate arguments.

    Differs from CreateArg in that there is no "init" field
    (they are replaced by SetA)
    """
    pass

class LowCreate(Item):
    """
    Create-sync or Create-detach block, with
    all complex expressions reduced to simple identifiers.

    Computed by visitor Create_2_LowCreate.
    """

    FUN_ID = 0
    FUN_PTR = 1

    def __init__(self, label = None, sync_type = None, 
                 target_next = None,
                 flavor = None,
                 body = None, args = None, 
                 *a, **kwargs):
        super(LowCreate, self).__init__(*a, **kwargs)

        self.flavor = flavor
        self.label = label
        self.place = "__slC_place_%s" % label
        self.start = "__slC_start_%s" % label
        self.limit = "__slC_limit_%s" % label
        self.step = "__slC_step_%s" % label
        self.block = "__slC_block_%s" % label
        self.retval = "__slC_ret_%s" % label
        self.target_resolved = "__slT_fini_%s" % label
        self.target_next = target_next
        self.sync_type = sync_type
        self.body = body
        self.funtype = None
        self.fun = '__slC_fun_%s' % label
        if args is None:
            args = []
        self.args = args

    @staticmethod
    def arg_init_var(name):
        return "__slAi_%s" % name

    @staticmethod
    def arg_var(name):
        return "__slA_%s" % name

class EndThread(Item):
    pass

class Break(Item):
    pass

class IndexDecl(Item):
    def __init__(self, indexname = None, *args, **kwargs):
        super(IndexDecl, self).__init__(*args, **kwargs)
        self.indexname = indexname

