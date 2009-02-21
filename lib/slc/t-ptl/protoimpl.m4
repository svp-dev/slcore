# ###############################################
#  Macro definitions for the muTC-ptl syntax
# ###############################################

# Pass transparently thread definitions.
m4_define([[ut_def]],[[m4_dnl
m4_define([[_ut_crcnt]],0)m4_dnl
m4_define([[ut_thparms]],[[m4_shiftn(2,$@)]])m4_dnl
void [[$1]]m4_ifelse((ut_thparms),(),(void),(ut_thparms))m4_dnl
]])

# No special action at the end of a definition
m4_define([[ut_enddef]],[[]])

# With the corecc syntax, a declaration looks the same as a definition.
m4_define([[ut_decl]], m4_defn([[ut_def]]))

# Pass transparently parameter declarations.
m4_define([[ut_shparm]], [[uTC::shared<[[$1]]>& __p_[[$2]]]])
m4_define([[ut_glparm]], [[uTC::global<[[$1]]>& __p_[[$2]]]])

# Pass transparently the index declaration.
m4_define([[ut_index]], [[uTC::index [[$1]]]])

# Pull shared and global argument declarations.
m4_define([[ut_sharg]],[[&::[[$1]]:[[$2]]:m4_ifelse([[$3]],,,[[= $3]])]])
m4_define([[ut_glarg]],[[:const:[[$1]]:[[$2]]:m4_ifelse([[$3]],,,[[= $3]])]])


m4_define([[ut_pulldecls]],[[m4_dnl
m4_ifelse([[$1]],,,[[m4_dnl
m4_regexp([[$1]],[[\([^:]*\):\([^:]*\):\([^:]*\):\([^:]*\):\([^:]*\)]],[[\3 \2 __a_\4 \5;]]) m4_dnl
$0(m4_shift($@))m4_dnl
]])m4_dnl
]])

m4_define([[ut_pullargs]],[[m4_dnl
m4_ifelse([[$1]],,,[[m4_dnl
,m4_regexp([[$1]],[[\([^:]*\):\([^:]*\):\([^:]*\):\([^:]*\):\([^:]*\)]],[[\1(__a_\4)]])m4_dnl
$0(m4_shift($@))m4_dnl
]])m4_dnl
]])

m4_define([[ut_create]], [[m4_dnl
m4_define([[_ut_crcnt]],m4_incr(_ut_crcnt))m4_dnl
m4_define([[_ut_lbl]],__child[[]]_ut_crcnt)m4_dnl
m4_define([[_ut_fid]],m4_ifelse([[$1]],,_ut_lbl,[[$1]]))m4_dnl
m4_define([[_ut_brk]],_ut_fid[[]]_brk)m4_dnl
m4_define([[_ut_place]],m4_ifelse([[$2]],,[[PLACE_LOCAL]],[[$2]]))m4_dnl
m4_define([[_ut_start]],m4_ifelse([[$3]],,0,[[$3]]))m4_dnl
m4_define([[_ut_limit]],m4_ifelse([[$4]],,1,[[$4]]))m4_dnl
m4_define([[_ut_step]],m4_ifelse([[$5]],,1,[[$5]]))m4_dnl
m4_define([[_ut_block]],m4_ifelse([[$6]],,0,[[$6]]))m4_dnl
m4_define([[_ut_brkref]],m4_ifelse(ut_breakable([[$7]]),1,[[_ut_brk[[,]]]],[[]]))m4_dnl
family _ut_fid; m4_dnl
m4_ifelse(ut_breakable([[$7]]),1,[[[[$7]] _ut_brk;]],) m4_dnl
ut_pulldecls(m4_shiftn(8,$@))m4_dnl
uTC::create(_ut_fid,_ut_place,_ut_start,_ut_limit,_ut_step,_ut_block,_ut_brkref NULL,m4_dnl
&([[$8]])ut_pullargs(m4_shiftn(8,$@)))m4_dnl
]])


# Pass transparently the sync construct.
m4_define([[ut_sync]],[[m4_dnl
m4_ifelse([[$2]],,,[[$2 = ]])uTC::sync(m4_ifelse([[$1]],,_ut_fid,[[$1]]))m4_dnl
]])

# Pass transparently all references to argument/parameter
# names.
m4_define([[ut_geta]],[[__a_[[$1]]]])
m4_define([[ut_seta]],[[__a_[[$1]] = $2]])
m4_define([[ut_getp]],[[__p_[[$1]]]])
m4_define([[ut_setp]],[[__p_[[$1]] = [[$2]]]])

# Pass transparently break and kill
m4_define([[ut_break]],[[uTC::Break($1)]])
m4_define([[ut_kill]],[[kill($1)]])

# Pass transparently the family id
m4_define([[ut_getfid]],[[$1]])

# Pass transparently the break id
m4_define([[ut_getbr]],[[$1]]_brk)

# ## End macros for core compiler syntax ###
