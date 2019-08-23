

from ..visitors import ScopedVisitor, DefaultVisitor, flatten
from ..ast import *

class Create_2_LowCreate(DefaultVisitor):

    def __init__(self, signed_ix):
        super(Create_2_LowCreate, self).__init__()
        self.signed_ix = signed_ix

    def visit_create(self, cr):

        lc = LowCreate(loc = cr.loc,
                       loc_end = cr.loc_end,
                       label = cr.label)
        
        newbl = []
        decls = cr.scope.decls

        cr.cvar_exitcode = CVarDecl(loc = cr.loc_end, name = 'C$R$%s' % cr.label, ctype = 'long', attrs=['unused'])
        decls += cr.cvar_exitcode
        cr.cvar_fid = CVarDecl(loc = cr.loc, name = 'C$F$%s' % cr.label, ctype = 'long')
        decls += cr.cvar_fid
        cr.cvar_extra = CVarDecl(loc = cr.loc, name = 'C$X$%s' % cr.label, ctype = 'long')
        decls += cr.cvar_extra

        smod = ''
        if not self.signed_ix:
            smod = 'unsigned'
        for item in ('place', 'start', 'limit', 'step', 'block'):
            if item != 'place':
                ctype = smod + ' long'
            else:
                ctype = 'long'
                
            var = CVarDecl(loc = cr.loc, name = 'C$%s$%s' % (item, cr.label), ctype = ctype)

            decls += var

            setattr(cr, 'cvar_%s' % item, var)

            newbl.append(CVarSet(loc = cr.loc, decl = var, rhs = getattr(cr, item)) + ';')
            
        if cr.funtype == cr.FUN_OPAQUE:
            cr.funtype = cr.FUN_VAR
            var = CVarDecl(loc = cr.loc, name = 'C$F$%s' % cr.label, ctype = 'void *')
            decls += var
            newbl.append(CVarSet(loc = cr.loc, decl = var, 
                                 rhs = CCast(ctype = 'void *',
                                             expr = cr.fun.accept(self))) + ';')
            cr.fun = var
        else:
            assert cr.funtype == cr.FUN_ID

        newbody = Block(loc = cr.body.loc, loc_end = cr.body.loc_end)
        newargs = []
        for a in cr.args:
            if a.type == 'xarg':
                argvar = cr.cvar_extra
            else:
                argvar = CVarDecl(loc = a.loc, name = 'C$a$%s' % a.name, ctype = a.ctype)
                decls += argvar
                newargs.append(a)
            a.cvar = argvar

            if a.init is not None:
                if a.type == 'xarg':
                    newbl.append(CVarSet(loc = a.loc, decl = cr.cvar_extra,
                                         rhs = a.init.accept(self)) + ';')
                else:
                    initvar = CVarDecl(loc = a.loc, name = 'C$ai$%s' % a.name, ctype = a.ctype)

                    decls += initvar
                    a.cvar_init = initvar

                    newbl.append(CVarSet(loc = a.loc, decl = initvar, 
                                     rhs = a.init.accept(self)) + ';')

                    seta = SetA(loc = a.loc, name = a.name, 
                                rhs = CVarUse(decl = initvar))
                    seta.decl = a
                    seta.scope = cr.scope
                    a.seen_set = True
                    newbody += (Opaque(';') + seta)
        cr.args = newargs

        if cr.fid_lvalue is not None:
            newbody += (flatten(cr.loc, "; (") + cr.fid_lvalue.accept(self) + ') = ' +
                        CVarUse(loc = cr.loc, decl = cr.cvar_fid))

        lc.body = newbody
        lc.body += cr.body.accept(self)

        newbl.append(lc)
        newbl.append(flatten(None, ';'))

        cr.target_resolved = CLabel(loc = cr.loc_end, name = 'Ce$%s' % cr.label) 
        newbl.append(cr.target_resolved)

        if cr.result_lvalue is not None:
            newbl.append(flatten(cr.loc_end, "; (") +
                         cr.result_lvalue.accept(self) +
                         ') = ' +
                         CVarUse(loc = cr.loc, decl = cr.cvar_exitcode))

        return newbl

class AutoResolve(ScopedVisitor):

    def visit_lowcreate(self, lc):
        cr = self.cur_scope.creates[lc.label]
        return lc + ';' + CGoto(loc = cr.loc_end, target = cr.target_resolved)

__all__ = ["Create_2_LowCreate", "AutoResolve"]
