from ..visitors import DefaultVisitor, ScopedVisitor, flatten
from ..ast import *
from ..msg import warn

#### Sequential transforms ####

class SSync_2_CSSync(DefaultVisitor):

    def visit_spawnsync(self, ss):
        return []

class Create_2_ptlCreate(ScopedVisitor):

    def __init__(self, *args, **kwargs):
        super(Create_2_ptlCreate, self).__init__(*args, **kwargs)
        self.__shlist = None
        self.__gllist = None

    def visit_seta(self, seta):
        b = seta.rhs.accept(self)
        return CVarSet(loc = seta.loc, decl = seta.decl.cvar, rhs = b) 

    def visit_createarg(self, arg):
        if self.__shlist or self.__gllist:
            self.__protolist += ','
        # prepare proto and uses
        if arg.type.startswith("sh"):
            self.__callist.append(flatten(None, ', &'))
            self.__callist.append(CVarUse(decl = arg.cvar))
            self.__protolist += flatten(None, 'uTC::shared< ') + arg.ctype + flatten(None, ' >&')
        else:
            self.__callist.append(flatten(None, ', '))
            self.__callist.append(CVarUse(decl = arg.cvar))
            self.__protolist += flatten(None, 'uTC::global< ') + arg.ctype + flatten(None, ' >&')
        return arg


    def visit_lowcreate(self, lc):
        #print "IN LOWC (v = %x, d = %x, lc = %x)" % (id(self), id(self.__dict__), id(lc))
        cr = self.cur_scope.creates[lc.label]

		# compute the calling convention
		# c = regmagic.mapcall(cr.args, funcname = "create", loc = cr.loc)

        self.__callist = []
        self.__protolist = Block()

        for a in cr.args:
		    a.accept(self)
        callist = self.__callist
        protolist = self.__protolist

        newbl = Block(loc = cr.loc)
        lbl = cr.label
        
		# generate allocate + test for alternative
        fidvar = cr.cvar_fid
        start  = CVarUse(decl = cr.cvar_start)
        limit  = CVarUse(decl = cr.cvar_limit)
        step   = CVarUse(decl = cr.cvar_step)
        block  = CVarUse(decl = cr.cvar_block)
        
        usefvar = CVarUse(decl = fidvar)

        if cr.extras.has_attr('exclusive'):
            allocmode = '3'
        elif lc.target_next is None:
            if cr.extras.has_attr('nowait'):
                warn("this create may fail and no alternative is available", cr)
                allocmode = '1'
            else:
                allocmode = '2'
        else:
            if cr.extras.has_attr('forcewait'):
                allocmode = '2'
            else:
                allocmode = '1'
	
     	strategyuse = CVarUse(cr.cvar_strategy)	

        newbl += (usefvar + ' = (long)(void*)uTC::allocate(' + allocmode +
						   ', ' + CVarUse(decl = cr.cvar_place) + ', ' + strategyuse + ');')
        
        if lc.target_next is not None:
            newbl += (flatten(cr.loc, ' if (!__builtin_expect(!!(') + 
                      usefvar + '), 1)) ' + 
                      CGoto(target = lc.target_next)) + ';'

        # generate the function pointer
        if cr.funtype == cr.FUN_ID:
            if lc.lowfun is not None:
                funvar = lc.lowfun
            else:
                # not yet split
                funvar = Opaque(cr.fun)
        else:
            assert cr.funtype == cr.FUN_VAR

            n = 'C$mtF$%s' % lbl
            t = 'C$mtF$%s' % lbl
            
            thetype = CTypeDecl(loc = cr.loc,
                                name = t,
                                ctype = CType(items = 
                                              Opaque(text = "void (*") +
                                              CTypeHead() + 
                                              ')(' + protolist + ')'))
            self.cur_scope.decls += thetype
            funvar = CVarDecl(loc = cr.loc, name = n, ctype = CTypeUse(tdecl = thetype))
            self.cur_scope.decls += funvar

            if lc.lowfun is not None:
                thefun = lc.lowfun
            else:
                # not yet split
                thefun = CVarUse(decl = cr.fun)

            newbl += CVarSet(loc = cr.loc, decl = funvar, 
                             rhs = CCast(ctype = CTypeUse(tdecl = thetype),
                                         expr = thefun)) + ';'
            funvar = CVarUse(decl = funvar)
        

        # Process the code in between create and sync, before create
        # happens. Because in ptl code we assign shared variables before
        # create
        newbl += lc.body.accept(self)

        # generate create

        if cr.sync_type == 'detach':
            printmsg = ' DPRINT( "Detach create is called from function " << __func__ << "(" << __FILE__ << ":" << __LINE__ << ")" ); '
            newbl += (flatten(cr.loc, printmsg) + 'uTC::create(*(uTC::family*)(void*)&' + usefvar + ', ' +
                  '(uTC::context)(void*)' + usefvar + ', ' + 'false, true, ' +
                  start + ',' + limit + ',' + step + ',' + block
                  + ',' + funvar + callist + ');')
        else:
            printmsg = ' DPRINT( "Create is called from function " << __func__ << "(" << __FILE__ << ":" << __LINE__ << ")" ); '
            newbl += (flatten(cr.loc, printmsg) + 'uTC::create(*(uTC::family*)(void*)&' + usefvar + ', ' +
                 '(uTC::context)(void*)' + usefvar + ', ' + 'false, false, ' +
                 start + ',' + limit + ',' + step + ',' + block + ',' + funvar + callist + ');')


        # done with body, now handle sync

        printmsg = ' DPRINT( "Sync is called from function " << __func__ << "(" << __FILE__ << ":" << __LINE__ << ")" ); '
        newbl += flatten(cr.loc_end, printmsg) + 'uTC::sync(*(uTC::family*)(void*)&' + usefvar +');'

        return newbl

__all__ = ['Create_2_ptlCreate'] #, 'SSync_2_CSSync']
