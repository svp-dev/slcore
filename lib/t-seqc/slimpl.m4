# ###############################################
#  Macro definitions for the sequential C syntax
# ###############################################

m4_define([[_sl_crcnt]],0)

# Thread definitions

m4_define([[sl_shparm]], [[[[$1]] [[$2]]]])
m4_define([[sl_glparm]], [[[[$1]] const [[$2]]]])
m4_copy([[sl_shparm]],[[sl_shfparm]])
m4_copy([[sl_glparm]],[[sl_glfparm]])

m4_define([[sl_def]],[[m4_dnl
m4_define([[sl_thparms]],m4_dquote(m4_shift2($@)))m4_dnl
void [[$1]](struct sl_famdata * __sl_fam) { m4_dnl
struct { m4_dnl 
m4_if(sl_breakable([[$2]]),1,[[[[$2]] __breakv; ]],)m4_dnl
m4_foreach([[_sl_parm]],m4_quote(sl_thparms),[[_sl_parm;]])m4_dnl
} *__sl_args = __sl_fam->a; struct sl_famdata __sl_child; m4_dnl
]])

m4_define([[sl_enddef]],[[} m4_dnl
m4_ifdef([[_sl_increate]],[[m4_fatal(missing sync after create)]]) m4_dnl
]])

m4_define([[sl_decl]], [[void [[$1]](struct sl_famdata * __sl_fam)]])

m4_define([[sl_index]], [[register const int [[$1]] = (__sl_fam->ix)]])
m4_define([[sl_getp]],[[(__sl_args->[[$1]])]])
m4_define([[sl_setp]],[[(__sl_args->[[$1]]) = [[$2]]]])


m4_define([[sl_declarg]],[[m4_dnl
[[$1]] [[$2]]; m4_dnl
]])

m4_define([[sl_initarg]],[[ m4_dnl
m4_pushdef([[_sl_initializer]],m4_joinall([[:]],m4_shiftn(2,$@))) m4_dnl
m4_if(m4_quote(_sl_initializer),,,[[_sl_lbl[[]]_args.[[$2]] = (_sl_initializer);]]) m4_dnl
register [[$1]] *const __a_[[$2]] = &_sl_lbl[[]]_args.[[$2]]; m4_dnl
m4_popdef([[_sl_initializer]]) m4_dnl
]])

# The following create just ignores the SVP place.
m4_define([[sl_create]], [[m4_dnl
m4_ifdef([[_sl_increate]],[[m4_fatal(cannot nest create)]])m4_dnl
m4_define([[_sl_increate]],1)m4_dnl
m4_step([[_sl_crcnt]])m4_dnl
m4_define([[_sl_lbl]],__child[[]]_sl_crcnt)m4_dnl
m4_define([[_sl_start]],m4_if([[$3]],,0,[[$3]]))m4_dnl
m4_define([[_sl_limit]],m4_if([[$4]],,1,[[$4]]))m4_dnl
m4_define([[_sl_step]],m4_if([[$5]],,1,[[$5]]))m4_dnl
m4_define([[_sl_thargs]],m4_dquote(m4_shiftn(8,$@)))m4_dnl
struct { m4_dnl
m4_if(sl_breakable([[$7]]),1,[[[[$7]] __breakv; ]])m4_dnl
m4_foreach([[_sl_arg]],m4_quote(_sl_thargs),[[m4_apply([[sl_declarg]],m4_split(_sl_arg,:))]]) m4_dnl
} _sl_lbl[[]]_args; m4_dnl
m4_if(sl_breakable([[$7]]),1,[[[[$7]] *_sl_fid[[]]_brk = &_sl_lbl[[]]_args.__breakv; ]])m4_dnl
m4_foreach([[_sl_arg]],m4_quote(_sl_thargs),[[m4_apply([[sl_initarg]],m4_split(_sl_arg,:))]]) m4_dnl
__sl_fam->ch = &__sl_child; m4_dnl
m4_if([[$1]],,,[[([[$1]]) = &__sl_child;]]) m4_dnl
__sl_child.be = _sl_start; m4_dnl
__sl_child.li = _sl_limit; m4_dnl
__sl_child.st = _sl_step; m4_dnl
__sl_child.ch = 0; m4_dnl
__sl_child.ex = SVP_ENOERR; m4_dnl
__sl_child.f = ([[$8]]); m4_dnl
__sl_child.a = &_sl_lbl[[]]_args m4_dnl
]])


# Pass transparently shared and global argument declarations.
m4_define([[sl_sharg]],[[$1:$2:$3]])
m4_copy([[sl_sharg]],[[sl_glarg]])
m4_copy([[sl_sharg]],[[sl_shfarg]])
m4_copy([[sl_sharg]],[[sl_glfarg]])

# Pass transparently the sync construct.
m4_define([[sl_sync]],[[m4_dnl
m4_ifndef([[_sl_increate]],[[m4_fatal(sync without create)]]) m4_dnl
m4_define([[_sl_body]],[[{ m4_dnl
if (__sl_child.ex != SVP_ENOERR) break; m4_dnl
__sl_child.f(&__sl_child); m4_dnl
if (__sl_fam->ex == SVP_EKILLED) return; m4_dnl
} m4_dnl
]])m4_dnl
do { m4_dnl
if (!__sl_child.st)m4_dnl
 for (__sl_child.ix = __sl_child.be; m4_dnl
      ; __sl_child.ix += __sl_child.li) _sl_body m4_dnl
else if (__sl_child.st > 0)m4_dnl
 for (__sl_child.ix = __sl_child.be; m4_dnl
      __sl_child.ix < __sl_child.li; m4_dnl
      __sl_child.ix += __sl_child.st) _sl_body m4_dnl
else m4_dnl
for (__sl_child.ix = __sl_child.be; m4_dnl
     __sl_child.ix > __sl_child.li; m4_dnl
     __sl_child.ix += __sl_child.st) _sl_body m4_dnl
__sl_fam->ch = 0; m4_dnl
m4_if([[$1]],,,[[[[$1]] = __sl_child.ex;]]) m4_dnl
} while(0) m4_dnl
m4_undefine([[_sl_increate]]) m4_dnl
]])

# Pass transparently all references to argument/parameter
# names.
m4_define([[sl_geta]],[[(*__a_[[$1]])]])
m4_define([[sl_seta]],[[(*__a_[[$1]]) = [[$2]]]])

# Pass transparently break and kill
m4_define([[sl_break]],[[m4_dnl
do { __sl_args->__breakv = ([[$1]]); __sl_fam->ex = SVP_EBROKEN; return; } while(0)m4_dnl
]])

m4_define([[sl_kill]],[[m4_dnl
  do {m4_dnl
    struct sl_famdata *__sl_p;m4_dnl					
    for (__sl_p = ([[$1]]); __sl_p; __sl_p = __sl_p->ch)m4_dnl
      __sl_p->ex = SVP_EKILLED;m4_dnl
    if (__sl_fam->ex == SVP_EKILLED) return;m4_dnl
  } while(0)m4_dnl
]])


# Pass transparently sl_getbr
m4_define([[sl_getbr]],(*[[$1]]_brk))

# ## End macros for new muTC syntax ###
