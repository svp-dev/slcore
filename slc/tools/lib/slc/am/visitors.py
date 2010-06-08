import pprint
import pdb
from ..visitors import *
from ..ast import *


def gen_loop_fun_name(orig_name):
    return Opaque("_fam_") + orig_name

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
            assert False #TODO

        
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
        limit = CVarUse(decl = cr.cvar_limit) + " - 1"
        step = CVarUse(decl = cr.cvar_step)
        
        rrhs = flatten(cr.loc_end, 'map_fam(&') + gen_loop_fun_name(funvar) + ', ' + start \
                + ', ' + limit + ', NULL)'

        mapping_call = CVarSet(decl = mapping_decision_var, rhs = rrhs)  #TODO: add step here
        newbl.append(mapping_call + ';\n')

        #expand call to allocate_fam()
        fam_context_var = CVarDecl(loc = cr.loc_end, name = 'alloc$%s' % lbl,
                                   ctype = 'fam_context_t*')
        self.cur_scope.decls += fam_context_var

        no_shareds = str(self.get_num_shareds())
        no_globals = str(self.get_num_globals())
       
        rrhs = flatten(cr.loc_end, 'allocate_fam(&') + gen_loop_fun_name(funvar) + ', ' \
                + no_shareds + ', ' +  no_globals + ', ' + start + ', ' \
                + limit + ', NULL, &' + CVarUse(decl = mapping_decision_var) + ')'
        allocate_call = CVarSet(decl = fam_context_var, 
                               rhs = rrhs)  #TODO: add step here
        newbl.append(allocate_call + ';\n')

        #expand call to create_fam()
        first_tc_var = CVarDecl(loc = cr.loc_end, name = 'first_tc$%s' % lbl,
                                ctype = 'tc_ident_t')
        self.cur_scope.decls += first_tc_var

        create_call = CVarSet(decl = first_tc_var,
                        rhs = flatten(cr.loc_end, 'create_fam(') + CVarUse(decl =
                                fam_context_var) + ')');
        newbl.append(create_call + ';\n')

        self.__first_tc = CVarUse(decl = first_tc_var)
        self.__fam_context = CVarUse(decl = fam_context_var)  
        
        # consume body
        newbl.append(lc.body.accept(self))
        
        #consume arguments
        for a in cr.args:
            a.accept(self) # accumulate the call/protolists/setters

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
        sync_call += ')'
        
        sync_call_assignment = CVarSet(decl = cr.cvar_exitcode, rhs = sync_call)
        
        newbl.append(sync_call_assignment)
                                        
        return newbl

class TFun_2_HydraCFunctions(DefaultVisitor):
    def __init__(self, *args, **kwargs):
        super(TFun_2_HydraCFunctions, self).__init__(*args, **kwargs)
        self.__shlist = None
        self.__gllist = None


    def visit_getp(self, getp):
        newbl = []
        param = self.__paramNames_2_params[getp.name]
        if not param.isShared:
            newbl.append(flatten(None,
                        'read_istruct_same_tc(&cur_tc->globals[%d])',
                        param.index))
        else: #shared
            if self.__state == 0: #begin
                newbl.append(flatten(None,#getp.loc,
                            'read_istruct(&cur_tc->shareds[%d], prev)' %
                            param.index))
            elif self.__state == 1: #middle
                newbl.append(flatten(None,#getp.loc,
                            'read_istruct_same_tc(&cur_tc->shareds[%d])' %
                            param.index))
            elif self.__state == 2: #end
                newbl.append(flatten(None,#getp.loc,
                            'read_istruct_same_tc(&cur_tc->shareds[%d])' %
                            param.index))
            elif self.__state == 3: #generic
                newbl.append(flatten(None,#getp.loc,
                            'read_istruct(&cur_tc->shareds[%d], prev)' %
                            param.index))
            else:
               assert False

        return newbl

        #print 'GETP'
        #print type(getp)
        #print '%s - %s' % (type(getp), getp.name)
        #pdb.set_trace()

        #if getp.name in self.__shlist:
        #    format = "(*__slP_%s)"
        #else:
        #    format = " __slP_%s "
        #return flatten(getp.loc, format % getp.name)

    def visit_setp(self, getp):
        b = getp.rhs.accept(self)
        param = self.__paramNames_2_params[getp.name]
        newbl = []

        #assert we're writing to a shared... TODO: check if it is possible to
        #write to a global (from a child) and, if so, what it's supposed to mean
        assert(param.isShared)

        if self.__state == 0: #begin
            newbl.append(flatten(None,#getp.loc,
                         'write_istruct_same_tc(&cur_tc->shareds[%d],' %
                          param.index) + b + ')')
            
        elif self.__state == 1: #middle
            newbl.append(flatten(None,#getp.loc,
                         'write_istruct_same_tc(&cur_tc->shareds[%d],' %
                          param.index) + b + ')')
        elif self.__state == 2 or self.__state == 3: #end and generic
            newbl.append(flatten(None,#getp.loc,
                         'if (_is_last_tc()) {shareds[%d] = ' % param.index) \
                         + b \
                         + ';} else {write_istruct(&next->tc->shareds[%d], ' % param.index \
                         + b + ', next);}')
        else:
            assert(0)

        return newbl
        #if getp.name in self.__shlist:
        #    format = "(*__slP_%s) = "
        #else:
        #    format = " __slP_%s = "
        #return [flatten(getp.loc, format % getp.name), b]

    def visit_funparm(self, parm):
        #print 'funparm'
        #print '%s - %s' % (type(parm), parm.name)

        self.__paramNames_2_params[parm.name] = parm
        if parm.type.startswith("sh"):
            parm.isShared = True
            parm.index = self.__sh_parm_index
            self.__sh_parm_index += 1
            #self.__shlist.append(parm.name)
            #self.__buffer += (Opaque(', register ') + parm.ctype + 
            #                  ' * const __restrict__ __slP_%s ' % parm.name)
        else:
            parm.isShared = False
            parm.index = self.__gl_parm_index
            self.__gl_parm_index += 1
            #self.__gllist.append(parm.name)
            #if parm.type.endswith('_mutable'):
            #    const = ""
            #else:
            #    const = "const"
            #self.__buffer += (Opaque(', register ') + parm.ctype + 
            #                  ' %s __slP_%s ' % (const, parm.name))
        return parm

    def visit_fundecl(self, fundecl, keep = False, omitextern = False):
        self.__sh_parm_index = 0
        self.__gl_parm_index = 0
        self.__paramNames_2_params = {}
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
        generic_body = copy.deepcopy(fundef.body)
      
        #TODO: function declaration... 
        #newitems = self.dispatch(fundef, seen_as = FunDecl, keep = True, omitextern = True)
        self.dispatch(fundef, seen_as = FunDecl, keep = True, omitextern = True)
 
        #TODO: maybe special case for t_main...
        
        self.__state = 0  #begin
        newitems = flatten(fundef.loc, ("long %s_begin(const tc_ident_t* prev, " +
                                       "long __index) {\n") % fundef.name)
        newitems += begin_body.accept(self)
        newitems += flatten(fundef.loc_end, "return 0; \n}")

        self.__state = 1  #middle
        newitems += flatten(fundef.loc, "long %s_middle(long __index) {\n" %
                                        fundef.name)
        newitems += middle_body.accept(self)
        newitems += flatten(fundef.loc_end, "return 0; \n}")

        self.__state = 2  #end
        newitems += flatten(fundef.loc, ("long %s_end(const tc_ident_t* next, " +
                            "long* shareds, long __index) {\n") % fundef.name)
        newitems += end_body.accept(self)
        newitems += flatten(fundef.loc_end, "return 0; \n}")
        
        self.__state = 3  #generic
        newitems += flatten(fundef.loc, ("long %s_generic(const tc_ident_t* " +
                            "prev, const tc_ident_t* next, " +
                            "long* shareds, long __index) {\n") % fundef.name)
        newitems += generic_body.accept(self)
        newitems += flatten(fundef.loc_end, "return 0; \n}")
       
        # generate loop function

        print '!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!'
        print type(fundef)
        print type(fundef.name)
        print fundef.name
    
        newitems += flatten(fundef.loc, "void ") + \
                    gen_loop_fun_name(fundef.name) + "()" +" {\n"
        newitems += "long __index, __start_index = _get_start_index(), " +\
            "__end_index = _get_end_index();\n"
        newitems += """
            fam_context_t* fam_context = _get_fam_context();
            const tc_ident_t* parent = _get_parent_ident();
            const tc_ident_t* prev = _get_prev_ident();
            const tc_ident_t* next = _get_next_ident();
            
            if (__end_index - __start_index > 4) {\n
            """ \
            + fundef.name + """_begin(prev, __start_index);
                for (__index = __start_index + 1; __index < __end_index; ++__index) {
                """ + fundef.name + """_middle(__index); // TODO: check for break return value
                }
            """ \
            + fundef.name + """_end(next, &fam_context->shareds[0], __end_index);
            } else {
                for (__index = __start_index; __index <= __end_index; ++__index) {
                """ + fundef.name + """_generic(prev, next, 
                        &fam_context->shareds[0], __index); // TODO: check for break value
                }
            }
            if (_is_last_tc() && parent->node_index != -1) write_istruct(&fam_context->done, 1, parent);

            cur_tc->finished = 1;
        """
        if fundef.name <> 't_main':
            newitems += "_return_to_scheduler();\n"
        else:
            newitems += "end_main();"
        newitems += """
        }
        """ 

        self.__shlist = self.__gllist = None
        return newitems

        #newitems = self.dispatch(fundef, seen_as = FunDecl, keep = True, omitextern = True)
        #newitems += flatten(fundef.loc, "{")
        #newitems += fundef.body.accept(self)
        #newitems += flatten(fundef.loc_end, " return 0; }")
        #self.__shlist = self.__gllist = None
        #return newitems


    def visit_indexdecl(self, idecl):
        return flatten(idecl.loc, "register const long %s = __index" %
                       idecl.indexname)
        #return flatten(idecl.loc, 
        #               " register const long %s = __slI " 
        #               % idecl.indexname) 

    def visit_break(self, br):
        return flatten(br.loc, " return 1 ")

    def visit_endthread(self, et):
        return flatten(et.loc, " return 0 ")


    def visit_program(self, p):
        # TODO: remove the absolute path below
        p._items.insert(0, Opaque("#include \"/home/amatei/src/thesis/trunk/runtime/rt.h\""))
        super(TFun_2_HydraCFunctions, self).visit_program(p)  # TODO: do I need
                                                              # this call?
        return p


__all__ = ["Create_2_HydraCall", "TFun_2_HydraCFunctions"]


