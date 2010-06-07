import pprint
import pdb
from ..visitors import *
from ..ast import *

class Create_2_HydraCall(ScopedVisitor):

    def __init__(self, *args, **kwargs):
        super(Create_2_HydraCall, self).__init__(*args, **kwargs)
      
    def get_num_shareds(self):
        rez = 0
        for a in self.__cur_cr.args:
            if (a.type.startswith("sh")):
                rez += 1
        return rez
    
    def get_num_globals(self):
        rez = 0
        for a in self.__cur_cr.args:
            if (not(a.type.startswith("sh"))):
                rez += 1
        return rez

    def get_arg_index(self, arg, shared):
        index = 0
        for a in self.__cur_cr.args:
            if (a == arg):
                return index
            else:
                if shared and a.type.startswith("sh"):
                    index += 1
                else:
                    if (not(shared)) and (not(a.type.startswith("sh"))):
                        index += 1
        assert(0)

    def visit_seta(self, seta):
        b = seta.rhs.accept(self)
        setter = None
        if seta.decl.type.startswith("sh"):
            # find my own index
            index = self.get_arg_index(arg = seta.decl, shared = True)
            setter = (flatten(seta.loc, "write_istruct(&") +
                self.__first_tc + ('.tc->shareds[%d], ' % index) + b +
                ', &' + self.__first_tc + ');\n')
        else:
            # find my own index
            index = self.get_arg_index(arg = seta.decl, shared = False)
            setter = (flatten(seta.loc, "write_global(&") +
                self.__fam_context + ', ' + index + ', ' + b + ');\n')
                

        self.__arg_setters.append(setter)
        return None
        #return CVarSet(loc = seta.loc, decl = seta.decl.cvar, rhs = b) 

    '''
    def visit_geta(self, geta):
        #pprint.pprint(object = geta, depth = 1)
        if geta.decl.type.startswith("sh"):
            shared = True
            # find my own index
            index = self.get_arg_index(arg = geta.decl, shared = True)
        else:
            shared = False
            index = self.get_arg_index(arg = geta.decl, shared = True)
        
        if shared:
            # generate the name of the local variable that was introduced for
            # the shared
            return CVarUse(decl = geta.decl.cvar)
        else:
            pass
    '''

    def visit_createarg(self, arg):
        # prepare proto and uses
        if arg.type.startswith("sh"):
            self.__callist.append(flatten(None, ', &'))
            self.__callist.append(CVarUse(decl = arg.cvar))
            self.__protolist += flatten(None, ', ') + arg.ctype + flatten(None, ' *')
        '''
        else:
            self.__callist.append(flatten(None, ', '))
            self.__callist.append(CVarUse(decl = arg.cvar))
            self.__protolist += flatten(None, ', ') + arg.ctype
        '''
        return arg

    def visit_lowcreate(self, lc):
        cr = self.cur_scope.creates[lc.label]

        if lc.target_next is not None:
            warn("alternative %s not used)" %
                 lc.target_next.name, lc)


        newbl = []
        lbl = cr.label

        # generate the function pointer
        if cr.funtype == cr.FUN_ID:
            if lc.lowfun is not None:
                funvar = lc.lowfun
            else:
                # not yet split
                funvar = Opaque(cr.fun)
        else:
            assert 0 #TODO

        
        # initialize members to be used when visiting the arguments
        cr = self.cur_scope.creates[lc.label]
        self.__callist = []
        self.__protolist = Block()
        #self.__shareds = []
        self.__arg_setters = []
        self.__cur_cr = cr

        # expand call to map_fam(..)
        mapping_decision_var = CVarDecl(loc = cr.loc_end, name =
                'map$%s' % lbl, ctype = 'mapping_decision')
        self.cur_scope.decls += mapping_decision_var
        
        start = CVarUse(decl = cr.cvar_start)
        limit = CVarUse(decl = cr.cvar_limit)
        step = CVarUse(decl = cr.cvar_step)
        
        rrhs = flatten(cr.loc_end, 'map_fam(&') + funvar + ', ' + start \
                + ', ' + limit + ', NULL)'

        mapping_call = CVarSet(decl = mapping_decision_var, rhs = rrhs)  #TODO: add step here
        newbl.append(mapping_call + ';\n')

        #expand call to allocate_fam()
        fam_context_var = CVarDecl(loc = cr.loc_end, name = 'alloc$%s' % lbl,
                                   ctype = 'fam_context_t*')
        self.cur_scope.decls += fam_context_var

        no_shareds = str(self.get_num_shareds())
        no_globals = str(self.get_num_globals())
       
        rrhs = flatten(cr.loc_end, 'allocate_fam(&') + funvar + ', ' \
                + no_shareds + ', ' +  no_globals + ', ' + start + ', ' \
                + limit + ', NULL, ' + CVarUse(decl = mapping_decision_var) + ')'
        allocate_call = CVarSet(decl = fam_context_var, 
                               rhs = rrhs)  #TODO: add step here
        newbl.append(allocate_call + ';\n')

        #expand call to create_fam()
        first_tc_var = CVarDecl(loc = cr.loc_end, name = 'first_tc$%s' % lbl,
                                ctype = 'const tc_ident_t')
        self.cur_scope.decls += first_tc_var

        create_call = CVarSet(decl = first_tc_var,
                        rhs = flatten(cr.loc_end, 'create_fam(') + CVarUse(decl =
                                fam_context_var) + ')');
        newbl.append(create_call + ';\n')

        self.__first_tc = CVarUse(decl = first_tc_var)
        self.__fam_context = CVarUse(decl = fam_context_var)  
        
        # consume body
        print 'Visiting body'
        newbl.append(lc.body.accept(self))
        print 'Done visiting body'
        
        #consume arguments
        print 'Visiting arguments'
        for a in cr.args:
            a.accept(self) # accumulate the call/protolists/setters
        print 'Done visiting arguments'

        #add setters for shareds and globals
        for s in self.__arg_setters:
            newbl.append(s);    
        

        #expand call to sync
        if not(self.__callist):
            self.__callist.append(flatten(None, ", NULL"))

        sync_call = flatten(cr.loc_end, "sync_fam(") + \
                    CVarUse(decl = fam_context_var) + ', ' + str(no_shareds) # + \
        for node in self.__callist:
            sync_call += node
        sync_call += ');\n'
        newbl.append(sync_call);   
                                        
        return newbl

class TFun_2_HydraCFunctions(DefaultVisitor):
    def __init__(self, *args, **kwargs):
        super(TFun_2_HydraCFunctions, self).__init__(*args, **kwargs)
        self.__shlist = None
        self.__gllist = None

    def visit_getp(self, getp):
        print 'GETP'
        #pdb.set_trace()
        if getp.name in self.__shlist:
            format = "(*__slP_%s)"
        else:
            format = " __slP_%s "
        return flatten(getp.loc, format % getp.name)

    def visit_setp(self, getp):
        b = getp.rhs.accept(self)
        if getp.name in self.__shlist:
            format = "(*__slP_%s) = "
        else:
            format = " __slP_%s = "
        return [flatten(getp.loc, format % getp.name), b]

    def visit_funparm(self, parm):
        if parm.type.startswith("sh"):
            self.__shlist.append(parm.name)
            self.__buffer += (Opaque(', register ') + parm.ctype + 
                              ' * const __restrict__ __slP_%s ' % parm.name)
        else:
            self.__gllist.append(parm.name)
            if parm.type.endswith('_mutable'):
                const = ""
            else:
                const = "const"
            self.__buffer += (Opaque(', register ') + parm.ctype + 
                              ' %s __slP_%s ' % (const, parm.name))
        return parm

    def visit_fundecl(self, fundecl, keep = False, omitextern = False):
        self.__shlist = []
        self.__gllist = []
        if fundecl.extras.get_attr('static', None) is not None:
            qual = "static"
        elif omitextern:
            qual = ""
        else:
            qual = "extern"
        self.__buffer = flatten(fundecl.loc, 
                                " %s long %s(const long __slI" 
                                % (qual, fundecl.name))
        for parm in fundecl.parms:
            parm.accept(self)
        self.__buffer += ')'
        ret = self.__buffer
        self.__buffer = None
        if not keep:
            self.__shlist = self.__gllist = None
        return ret
 
    def visit_fundeclptr(self, fundecl):
        assert False
        '''
        self.__shlist = []
        self.__gllist = []
        if fundecl.extras.get_attr('typedef', None) is not None:
            qual = "typedef"
        elif fundecl.extras.get_attr('static', None) is not None:
            qual = "static"
        else:
            qual = ''
        self.__buffer = flatten(fundecl.loc, 
                                " %s long (*%s)(const long __slI" 
                                % (qual, fundecl.name))
        for parm in fundecl.parms:
            parm.accept(self)
        self.__buffer += ')'
        ret = self.__buffer
        self.__buffer = None
        self.__shlist = self.__gllist = None
        return ret
        '''

    def visit_fundef(self, fundef):
        # make copies of the body for begin, middle and end

        begin_body = copy.deepcopy(fundef.body)
        middle_body = copy.deepcopy(fundef.body)
        end_body = copy.deepcopy(fundef.body)
      
        #TODO: function declaration... 
        #newitems = self.dispatch(fundef, seen_as = FunDecl, keep = True, omitextern = True)
        self.dispatch(fundef, seen_as = FunDecl, keep = True, omitextern = True)
 
        #TODO: maybe special case for t_main...
        
        newitems = flatten(fundef.loc, ("long %s_begin(const tc_ident_t* prev, " +
                                       "long __index) {\n") % fundef.name)
        self.__state = 0  #begin
        newitems += begin_body.accept(self)
        newitems += flatten(fundef.loc_end, "return 0; \n}")

        newitems += flatten(fundef.loc, "long %s_middle(long __index) {\n" %
                                        fundef.name)
        self.__state = 1  #middle
        newitems += middle_body.accept(self)
        newitems += flatten(fundef.loc_end, "return 0; \n}")

        newitems += flatten(fundef.loc, ("long %s_end(const tc_ident_t* next, " +
                            "long* shareds, long __index) {\n") % fundef.name)
        self.__state = 2  #end
        newitems += end_body.accept(self)
        newitems += flatten(fundef.loc_end, "return 0; \n}")
       
        # TODO: generate loop function
        newitems += flatten(fundef.loc, "void fam_%s() {\n" % fundef.name)
        newitems += "long __index, __start_index = get_start_index(), " +\
            "__end_index = get_end_index();\n"
        newitems += """
            fam_context_t* fam_context = get_fam_context();
            const tc_ident_t* parent = get_parent_ident();
            """ + fundef.name + """_begin(parent, __start_index);
            for (__index = __start_index + 1; __index < __end_index; ++__index) {
              """ + fundef.name + """_middle(__index); // TODO: check for break return value
            }
            """+ fundef.name + """_end(parent, &fam_context->shareds[0], __end_index);
            write_istruct(&fam_context->done, 1, parent);

            cur_tc->finished = 1;
            return_to_scheduler();
        }
        """ 

        self.__shlist = self.__gllist = None
        print 'WROTE functions'
        return newitems

        #newitems = self.dispatch(fundef, seen_as = FunDecl, keep = True, omitextern = True)
        #newitems += flatten(fundef.loc, "{")
        #newitems += fundef.body.accept(self)
        #newitems += flatten(fundef.loc_end, " return 0; }")
        #self.__shlist = self.__gllist = None
        #return newitems


    def visit_indexdecl(self, idecl):
        return flatten(idecl.loc, 
                       " register const long %s = __slI " 
                       % idecl.indexname) 

    def visit_break(self, br):
        return flatten(br.loc, " return 1 ")

    def visit_endthread(self, et):
        return flatten(et.loc, " return 0 ")





__all__ = ["Create_2_HydraCall", "TFun_2_HydraCFunctions"]


