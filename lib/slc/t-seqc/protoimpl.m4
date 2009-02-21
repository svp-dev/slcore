# ###############################################
#  Macro definitions for the sequential C syntax
# ###############################################

# Thread definitions

m4_define([[ut_shparm]], [[[[$1]] [[$2]]]])
m4_define([[ut_glparm]], [[[[$1]] [[$2]]]])

m4_define([[ut_declparms]], [[m4_dnl
m4_ifelse([[$1]],,,[[m4_dnl
[[$1]]; m4_dnl
$0(m4_shift($@))m4_dnl
]])m4_dnl
]])

m4_define([[ut_def]],[[m4_dnl
m4_define([[_ut_crcnt]],0)m4_dnl
m4_define([[ut_thparms]],[[m4_shiftn(2,$@)]])m4_dnl
void [[$1]](struct ut_famdata *__fam) { m4_dnl
struct { m4_dnl 
m4_ifelse(ut_breakable([[$2]]),1,[[[[$2]] __breakv; ]],)m4_dnl
ut_declparms(ut_thparms)m4_dnl
} *__args = __fam->a; struct ut_famdata __fid; m4_dnl
]])

m4_define([[ut_enddef]],[[}]])

m4_define([[ut_decl]], [[void [[$1]](struct ut_famdata *__fam)]])

m4_define([[ut_index]], [[const int [[$1]] = (__fam->ix)]])
m4_define([[ut_getp]],[[(__args->[[$1]])]])
m4_define([[ut_setp]],[[(__args->[[$1]]) = [[$2]]]])


m4_define([[ut_declargs]],[[m4_dnl
m4_ifelse([[$1]],,,[[m4_dnl
m4_regexp([[$1]],[[\([^:]*\):\([^:]*\):[^:]*]],[[\1 \2; ]])m4_dnl
$0(m4_shift($@))m4_dnl
]])m4_dnl
]])

m4_define([[ut_initargs]],[[m4_dnl
m4_ifelse([[$1]],,,[[m4_dnl
m4_define([[ut_arginit]],m4_regexp([[$1]],[[[^:]*:[^:]*:\([^:]*\)]],[[\1]]))m4_dnl
m4_ifelse((ut_arginit),(),,[[m4_regexp([[$1]],[[[^:]*:\([^:]*\):\([^:]*\)]],[[_ut_fid[[]]_args.\1 = (\2); ]])]])m4_dnl
m4_regexp([[$1]],[[\([^:]*\):\([^:]*\):[^:]*]],[[register \1 *const __a_\2 = &_ut_fid[[]]_args.\2; ]])m4_dnl
$0(m4_shift($@))m4_dnl
]])m4_dnl
]])

m4_define([[ut_create]], [[m4_dnl
m4_define([[_ut_crcnt]],m4_incr(_ut_crcnt))m4_dnl
m4_define([[_ut_lbl]],__child[[]]_ut_crcnt)m4_dnl
m4_define([[_ut_fid]],m4_ifelse([[$1]],,_ut_lbl,[[$1]]))m4_dnl
m4_define([[_ut_place]],m4_ifelse([[$2]],,[[PLACE_LOCAL]],[[$2]]))m4_dnl
m4_define([[_ut_start]],m4_ifelse([[$3]],,0,[[$3]]))m4_dnl
m4_define([[_ut_limit]],m4_ifelse([[$4]],,1,[[$4]]))m4_dnl
m4_define([[_ut_step]],m4_ifelse([[$5]],,1,[[$5]]))m4_dnl
m4_define([[ut_thargs]],[[m4_shiftn(8,$@)]])m4_dnl
struct { m4_dnl
m4_ifelse(ut_breakable([[$7]]),1,[[[[$7]] __breakv; ]])m4_dnl
ut_declargs(ut_thargs)m4_dnl
} _ut_fid[[]]_args; m4_dnl
m4_ifelse(ut_breakable([[$7]]),1,[[[[$7]] *_ut_fid[[]]_brk = &_ut_fid[[]]_args.__breakv; ]])m4_dnl
ut_initargs(ut_thargs)m4_dnl
register struct ut_famdata *_ut_fid = &__fid;m4_dnl
__fam->ch = _ut_fid; m4_dnl
__fid.be = _ut_start; m4_dnl
__fid.li = _ut_limit; m4_dnl
__fid.st = _ut_step; m4_dnl
__fid.ch = 0; m4_dnl
__fid.ex = EXIT_NORMAL; m4_dnl
__fid.f = ([[$8]]); m4_dnl
__fid.a = &_ut_fid[[]]_args m4_dnl
]])


# Pass transparently shared and global argument declarations.
m4_define([[ut_sharg]],[[$1:$2:$3]])
m4_define([[ut_glarg]],m4_defn([[ut_sharg]]))

# Pass transparently the sync construct.
m4_define([[ut_sync]],[[m4_dnl
m4_define([[_ut_body]],[[{ m4_dnl
if (__fid.ex != EXIT_NORMAL) break; m4_dnl
__fid.f(&__fid); m4_dnl
if (__fam->ex == EXIT_KILLED) return; m4_dnl
} m4_dnl
]])m4_dnl
do { m4_dnl
if (!__fid.st)m4_dnl
 for (__fid.ix = __fid.be; ; __fid.ix += __fid.li) _ut_body m4_dnl
else if (__fid.st > 0)m4_dnl
 for (__fid.ix = __fid.be; __fid.ix < __fid.li; __fid.ix += __fid.st) _ut_body m4_dnl
else m4_dnl
for (__fid.ix = __fid.be; __fid.ix > __fid.li; __fid.ix += __fid.st) _ut_body m4_dnl
__fam->ch = 0; m4_dnl
m4_ifelse([[$2]],,,[[$2 = __fid.ex;]])m4_dnl
} while(0)m4_dnl
]])

# Pass transparently all references to argument/parameter
# names.
m4_define([[ut_geta]],[[(*__a_$1)]])
m4_define([[ut_seta]],[[(*__a_$1) = $2]])

# Pass transparently break and kill
m4_define([[ut_break]],[[m4_dnl
do { __args->__breakv = ([[$1]]); __fam->ex = EXIT_BROKEN; return; } while(0)m4_dnl
]])

m4_define([[ut_kill]],[[kill($1)]])

# Pass transparently ut_getfid
m4_define([[ut_getfid]],[[$1]])

# Pass transparently ut_getbr
m4_define([[ut_getbr]],(*[[$1]]_brk))

# ## End macros for new muTC syntax ###
