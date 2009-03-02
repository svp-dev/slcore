# ###############################################
#  Macro definitions for the PPP-Alpha compiler
# ###############################################

m4_define([[_sl_crcnt]],0)

# Thread definitions

m4_define([[sl_shparm]], [[[[$1]] [[$2]]]])
m4_define([[sl_glparm]], [[[[$1]] [[$2]]]])

m4_define([[sl_declparms]], [[m4_dnl
m4_ifelse([[$1]],,,[[m4_dnl
 SHARG[[]]__sl_argcnt[[]]([[$1]]); m4_dnl
$0(m4_shift($@))m4_dnl
]])m4_dnl
]])

m4_define([[sl_def]],[[m4_dnl
m4_define([[__sl_funcname]],[[$1]])m4_dnl
BEGIN_FUNC([[$1]])m4_dnl
m4_define([[__sl_argcnt]],0)m4_dnl
m4_define([[sl_thparms]],[[m4_shiftn(2,$@)]])m4_dnl
sl_declparms(sl_thparms)m4_dnl
]])

m4_define([[sl_enddef]],[[
END_FUNC(__sl_funcname)
]])

m4_define([[sl_decl]], [[void [[$1]](void)]])

m4_define([[sl_index]], [[INDEX([[$1]])]])
m4_define([[sl_getp]],[[GETSH([[$1]])]])
m4_define([[sl_setp]],[[SETSH([[$1]], [[$2]])]])


m4_define([[sl_declargs]],[[m4_dnl
m4_ifelse([[$1]],,,[[m4_dnl
m4_regexp([[$1]],[[\([^:]*\):\([^:]*\):[^:]*]],[[\1 \2; ]])m4_dnl
$0(m4_shift($@))m4_dnl
]])m4_dnl
]])

m4_define([[sl_initargs]],[[m4_dnl
m4_ifelse([[$1]],,,[[m4_dnl
m4_define([[sl_arginit]],m4_regexp([[$1]],[[[^:]*:[^:]*:\([^:]*\)]],[[\1]]))m4_dnl
m4_ifelse((sl_arginit),(),,[[m4_regexp([[$1]],[[[^:]*:\([^:]*\):\([^:]*\)]],[[_sl_fid[[]]_args.\1 = (\2); ]])]])m4_dnl
m4_regexp([[$1]],[[\([^:]*\):\([^:]*\):[^:]*]],[[register \1 *const __a_\2 = &_sl_fid[[]]_args.\2; ]])m4_dnl
$0(m4_shift($@))m4_dnl
]])m4_dnl
]])

m4_define([[sl_create]], [[m4_dnl
m4_define([[_sl_crcnt]],m4_incr(_sl_crcnt))m4_dnl
m4_define([[_sl_lbl]],__child[[]]_sl_crcnt)m4_dnl
m4_define([[_sl_fid]],m4_ifelse([[$1]],,_sl_lbl,[[$1]]))m4_dnl
m4_define([[_sl_place]],m4_ifelse([[$2]],,[[PLACE_LOCAL]],[[$2]]))m4_dnl
m4_define([[_sl_start]],m4_ifelse([[$3]],,0,[[$3]]))m4_dnl
m4_define([[_sl_limit]],m4_ifelse([[$4]],,1,[[$4]]))m4_dnl
m4_define([[_sl_step]],m4_ifelse([[$5]],,1,[[$5]]))m4_dnl
m4_define([[sl_thargs]],[[m4_shiftn(8,$@)]])m4_dnl
struct { m4_dnl
m4_ifelse(sl_breakable([[$7]]),1,[[[[$7]] __breakv; ]])m4_dnl
sl_declargs(sl_thargs)m4_dnl
} _sl_fid[[]]_args; m4_dnl
m4_ifelse(sl_breakable([[$7]]),1,[[[[$7]] *_sl_fid[[]]_brk = &_sl_fid[[]]_args.__breakv; ]])m4_dnl
sl_initargs(sl_thargs)m4_dnl
register struct sl_famdata *_sl_fid = &__fid;m4_dnl
__fam->ch = _sl_fid; m4_dnl
__fid.be = _sl_start; m4_dnl
__fid.li = _sl_limit; m4_dnl
__fid.st = _sl_step; m4_dnl
__fid.ch = 0; m4_dnl
__fid.ex = SVP_ENOERR; m4_dnl
__fid.f = ([[$8]]); m4_dnl
__fid.a = &_sl_fid[[]]_args m4_dnl
]])


# Pass transparently shared and global argument declarations.
m4_define([[sl_sharg]],[[$1:$2:$3]])
m4_define([[sl_glarg]],m4_defn([[sl_sharg]]))

# Pass transparently the sync construct.
m4_define([[sl_sync]],[[m4_dnl
m4_define([[_sl_body]],[[{ m4_dnl
if (__fid.ex != SVP_ENOERR) break; m4_dnl
__fid.f(&__fid); m4_dnl
if (__fam->ex == SVP_EKILLED) return; m4_dnl
} m4_dnl
]])m4_dnl
do { m4_dnl
if (!__fid.st)m4_dnl
 for (__fid.ix = __fid.be; ; __fid.ix += __fid.li) _sl_body m4_dnl
else if (__fid.st > 0)m4_dnl
 for (__fid.ix = __fid.be; __fid.ix < __fid.li; __fid.ix += __fid.st) _sl_body m4_dnl
else m4_dnl
for (__fid.ix = __fid.be; __fid.ix > __fid.li; __fid.ix += __fid.st) _sl_body m4_dnl
__fam->ch = 0; m4_dnl
m4_ifelse([[$2]],,,[[$2 = __fid.ex;]])m4_dnl
} while(0)m4_dnl
]])

# Pass transparently all references to argument/parameter
# names.
m4_define([[sl_geta]],[[[[$1]]]])
m4_define([[sl_seta]],[[[[$2]] = [[$1]]]])

# Pass transparently break and kill
m4_define([[sl_break]],[[m4_dnl
m4_ifelse(__sl_breakt,[[int]],
[[__ppp_break([[$1]])]],
m4_ifelse(__sl_breakt,[[float]]
[[__ppp_breakf([[$1]])]],
[[#error cannot use break here
]]))m4_dnl
]])

m4_define([[sl_kill]],[[__ppp_kill([[$1]])]])


# Pass transparently sl_getfid
m4_define([[sl_getfid]],[[$1]])

# Pass transparently sl_getbr
m4_define([[sl_getbr]],[[$1]]_brk)

# ## End macros for new muTC syntax ###
