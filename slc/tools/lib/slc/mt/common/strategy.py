from ...visitors import DefaultVisitor, ScopedVisitor, flatten
from ...ast import *
from ...msg import warn,die

class CreateMTStrategy(ScopedVisitor):
 
    def __init__(self, newisa = False, *args, **kwargs):
        self.newisa = newisa
        super(CreateMTStrategy, self).__init__(*args, **kwargs)
   
    def visit_lowcreate(self, lc):

        lc.body.accept(self)

        # we need to prepare the strategy variable
        # even if strategies are disabled.

        cr = self.cur_scope.creates[lc.label]
        
        lbl = cr.label

        strategyvar = CVarDecl(loc = cr.loc, name = 'C$S$%s' % lbl, ctype = Opaque("int"), init=Opaque("0"))
        self.cur_scope.decls += strategyvar
        cr.cvar_strategy = strategyvar

        usestrategy = self.extra_options.get('create-strategies',True)

        sattr = cr.extras.get_attr('strategy', None)
        if sattr is None:
            sname = 'default'
        else:
            if usestrategy and not self.newisa:
                warn("attribute 'strategy' has no effect on this target", cr)

            sname = sattr.strategy

        cr.strategy = sname

        if not usestrategy:
            warn("attribute 'strategy' ignored", cr)
            return lc


        start = CVarUse(decl = cr.cvar_start)
        limit = CVarUse(decl = cr.cvar_limit)
        step = CVarUse(decl = cr.cvar_step)
        strategyuse = CVarUse(strategyvar)
        newbl = Block(loc = cr.loc)

        isdependent = False
        for a in cr.args:
            if a.type.startswith('sh'):
                isdependent = True

        if sname == 'single':
            newbl += strategyuse + " = 3; " 
        elif sname == 'balanced':
            newbl += strategyuse + " = 2; " # implies single 
        elif sname in ['default','fill','exact']:
            if sname == 'exact':
                newbl += strategyuse + " = 1; "
            elif isdependent:
                newbl += strategyuse + " = 3; "
            elif self.extra_options.get('collapse-singletons',True):
                newbl += (Opaque(' if (__builtin_constant_p(') + start + ') &&'
                          '__builtin_constant_p(' + limit + ') &&' 
                          '__builtin_constant_p(' + step + ') &&'
                          '((' + step + '> 0) ? (' + start + '+' + step + '>='+ limit + '):' 
                          '(' + start + '+' + step + '<=' + limit +'))) ' +
                          strategyuse + " = 3; ")
        newbl += lc
        return newbl
