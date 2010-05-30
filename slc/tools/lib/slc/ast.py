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

    def __add__(self, other):
        if isinstance(other, Item):
            loc_end = other.loc_end
        else:
            loc_end = None
        newbl = Block(loc = self.loc, loc_end = loc_end)
        if isinstance(self, Block):
            newbl += self._items
        else:
            newbl += self
        if isinstance(other, Block):
            newbl += other._items
        else:
            newbl += other
        return newbl

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

    def __init__(self, items = None, *args, **kwargs):
        super(Block, self).__init__(*args, **kwargs)
        #if isinstance(items, Block):
        #    items = items._items
        if items is None:
            items = []
        elif isinstance(items, str):
            items = [Opaque(text = items)]
        elif not isinstance(items, list):
            items = [items]
        self._items = items

    def __iadd__(self, item):
        #if isinstance(item, Block):
        #    self._items += item._items
        #el
        if isinstance(item, list):
            self._items += item
        elif isinstance(item, str):
            self._items.append(Opaque(item))
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

class Scope(Block):
    """
    A block surrounded by {}.
    """
    def __init__(self, decls = None, *args, **kwargs):
        super(Scope, self).__init__(*args, **kwargs)
        if not isinstance(decls, Block):
            decls = Block(items = decls)
        self.decls = decls
        self.creates = {}

class CTypeDecl(Item):
    """
    a C typedef declaration.
    """

    def __init__(self, name = None, ctype = None, *args, **kwargs):
        super(CTypeDecl, self).__init__(*args, **kwargs)
        self.name = name
        if not isinstance(ctype, CType):
            ctype = CType(items = ctype)
        self.ctype = ctype

class CVarDecl(Item):
    """
    A C variable declaration.
    """

    def __init__(self, name = None, ctype = None, init = None, *args, **kwargs):
        super(CVarDecl, self).__init__(*args, **kwargs)
        self.name = name
        if not isinstance(ctype, CType):
            ctype = CType(items = ctype)
        self.ctype = ctype
        if not isinstance(init, Block):
            init = Block(items = init)
        self.init = init

class CTypeUse(Item):
    """
    a C typedef name use.
    """
    

    def __init__(self, tdecl = None, *args, **kwargs):
        super(CTypeUse, self).__init__(*args, **kwargs)
        self.tdecl = tdecl

class CVarUse(Item):
    """
    a C variable use.
    """

    def __init__(self, decl = None, *args, **kwargs):
        super(CVarUse, self).__init__(*args, **kwargs)
        self.decl = decl


class CVarSet(Item):
    """
    a C variable assignment.
    """

    def __init__(self, decl = None, rhs = None, *args, **kwargs):
        super(CVarSet, self).__init__(*args, **kwargs)
        self.decl = decl

        if not isinstance(rhs, Block):
            rhs = Block(items = rhs)
        self.rhs = rhs

    #def __deepcopy__(self, memo = {}):
    #    new = super(CVarSet, self).__deepcopy__(memo)
    #    new.rhs = copy.deepcopy(self.rhs, memo)
    #    return new

class CLabel(Item):
    """
    a C label.
    """
    def __init__(self, name = None, *args, **kwargs):
        super(CLabel, self).__init__(*args, **kwargs)
        self.name = name

class CGoto(Item):
    """
    a C goto statement.
    """

    def __init__(self, target = None, *args, **kwargs):
        super(CGoto, self).__init__(*args, **kwargs)
        self.target = target

class CCast(Item):
    """
    a C cast expression.
    """

    def __init__(self, ctype = None, expr = None, *args, **kwargs):
        super(CCast, self).__init__(*args, **kwargs)
        if not isinstance(ctype, CType):
            ctype = CType(items = ctype)
        self.ctype = ctype
        if not isinstance(expr, Block):
            expr = Block(items = expr)
        self.expr = expr

class CTypeHead(Item):
    """
    The "center" of the declarator part of a C type string.
    """
    pass

class CType(Block):
    """
    a C type string.
    """
    pass


class CIndex(Item):
    """
    a C indexing expression.
    """

    def __init__(self, expr = None, index = None, *args, **kwargs):
        super(CIndex, self).__init__(*args, **kwargs)
        if not isinstance(expr, Block):
            expr = Block(items = expr)
        self.expr = expr
        if not isinstance(index, Block):
            index = Block(items = index)
        self.index = index
            

class Flavor(Block):
    """
    Flavor root for a branch of the AST.
    """

    def __init__(self, flavor = None, *args, **kwargs):
        super(Flavor, self).__init__(*args, **kwargs)
        self.flavor = flavor

_handle = []
class Extras(Block):
    """
    Extra annotations to function definitions/declarations
    and creates.
    """

    def get_attr(self, name, default = _handle):
        for a in self:
            if isinstance(a, Attr) and a.name == name:
                return a
        if default is _handle:
            raise KeyError, name
        return default

class Attr(Item):
    """
    some attribute in the extra annotations
    """

    def __init__(self, name, payload = None, *args, **kwargs):
        super(Attr, self).__init__(*args, **kwargs)
        self.name = name
        if payload is None:
            payload = {}
        for k, v in payload.items():
            setattr(self, k, v)
            
class Program(Block):
    """
    Root node for a program.
    """

    pass


class FunDeclBase(Item):

    def __init__(self, name = None, parms = None, extras = None, *args, **kwargs):
        super(FunDeclBase, self).__init__(*args, **kwargs)
        self._name = name

        if parms is None: parms = []
        self.parms = parms
        
        if extras is None: extras = Extras()
        self.extras = extras

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
        if not isinstance(ctype, CType):
            ctype = CType(items = ctype)
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
        if not isinstance(rhs, Block):
            rhs = Block(items = rhs)
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

    FUN_ID = 0
    FUN_VAR = 1
    FUN_OPAQUE = 2

    def __init__(self, label = None, place = None, 
                 start = None, step = None, limit = None, block = None,
                 sync_type = None, fun = None, body = None,
                 result_lvalue = None, extras = None,
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
        if extras is None:
            extras = Extras()
        self.extras = extras

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
        self._fun = fun
        if isinstance(fun, CVarDecl):
            self.funtype = self.FUN_VAR
        elif isinstance(fun, str):
            self.funtype = self.FUN_ID
        elif isinstance(fun, Block):
            if len(fun) == 1 and isinstance(fun[0], Opaque):
                (t, i) = lex.is_simple_identifier(fun[0].text)
                if t:
                    self._fun = i
                    self.funtype = self.FUN_ID
                    return
            self.funtype = self.FUN_OPAQUE
        else:
            self.funtype = None

class LowCreate(Item):
    """
    Create-sync or Create-detach block, with
    all complex expressions reduced to simple identifiers.

    Computed by visitor Create_2_LowCreate.
    """

    def __init__(self, 
                 label = None, 
                 body = None,
                 flavor = None,
                 target_next = None,
                 *args, **kwargs):
        super(LowCreate, self).__init__(*args, **kwargs)

        self.flavor = flavor
        self.label = label
        self.target_next = target_next

        if not isinstance(body, Block):
            body = Block(items = body)
        self.body = body
        
        self.lowfun = None

class EndThread(Item):
    pass

class Break(Item):
    pass

class IndexDecl(Item):
    def __init__(self, indexname = None, *args, **kwargs):
        super(IndexDecl, self).__init__(*args, **kwargs)
        self.indexname = indexname

