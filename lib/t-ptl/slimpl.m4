# ###############################################
#  Macro definitions for the muTC-ptl syntax
# ###############################################

m4_define([[_sl_crcnt]],0)

# Pass transparently thread definitions.
m4_define([[sl_def]],[[m4_dnl
m4_define([[sl_thparms]],[[m4_shiftn(2,$@)]])m4_dnl
void [[$1]]m4_if((sl_thparms),(),(void),(sl_thparms))m4_dnl
]])

# No special action at the end of a definition
m4_define([[sl_enddef]],[[]])

# With the corecc syntax, a declaration looks the same as a definition.
m4_define([[sl_decl]], m4_defn([[sl_def]]))

# Pass transparently parameter declarations.
m4_define([[sl_shparm]], [[uTC::shared<[[$1]]>& __p_[[$2]]]])
m4_define([[sl_glparm]], [[uTC::global<[[$1]]>& __p_[[$2]]]])
m4_copy([[sl_shparm]],[[sl_shfparm]])
m4_copy([[sl_glparm]],[[sl_glfparm]])

# Pass transparently the index declaration.
m4_define([[sl_index]], [[uTC::index [[$1]]]])

# Pull shared and global argument declarations.
m4_define([[sl_sharg]],[[&::[[$1]]:[[$2]]:m4_if([[$3]],,,[[= [[$3]]]])]])
m4_define([[sl_glarg]],[[:const:[[$1]]:[[$2]]:m4_if([[$3]],,,[[= [[$3]]]])]])
m4_copy([[sl_sharg]],[[sl_shfarg]])
m4_copy([[sl_glarg]],[[sl_glfarg]])


m4_define([[sl_pulldecls]],[[m4_dnl
m4_if([[$1]],,,[[m4_dnl
m4_bregexp([[$1]],[[\([^:]*\):\([^:]*\):\([^:]*\):\([^:]*\):\(.*\)]],[[\3 \2 __a_\4 \5;]]) m4_dnl
$0(m4_shift($@))m4_dnl
]])m4_dnl
]])

m4_define([[sl_pullargs]],[[m4_dnl
m4_if([[$1]],,,[[m4_dnl
,m4_bregexp([[$1]],[[\([^:]*\):\([^:]*\):\([^:]*\):\([^:]*\):\([^:]*\)]],[[\1(__a_\4)]])m4_dnl
$0(m4_shift($@))m4_dnl
]])m4_dnl
]])

m4_define([[sl_create]], [[m4_dnl
m4_define([[_sl_crcnt]],m4_incr(_sl_crcnt))m4_dnl
m4_define([[_sl_lbl]],__child[[]]_sl_crcnt)m4_dnl
m4_define([[_sl_fid]],m4_if([[$1]],,_sl_lbl,[[$1]]))m4_dnl
m4_define([[_sl_brk]],_sl_fid[[]]_brk)m4_dnl
m4_define([[_sl_place]],m4_if([[$2]],,[[SVP_LOCAL]],[[$2]]))m4_dnl
m4_define([[_sl_start]],m4_if([[$3]],,0,[[$3]]))m4_dnl
m4_define([[_sl_limit]],m4_if([[$4]],,1,[[$4]]))m4_dnl
m4_define([[_sl_step]],m4_if([[$5]],,1,[[$5]]))m4_dnl
m4_define([[_sl_block]],m4_if([[$6]],,0,[[$6]]))m4_dnl
m4_define([[_sl_brkref]],m4_if(sl_breakable([[$7]]),1,[[_sl_brk[[,]]]],[[]]))m4_dnl
sl_family_t _sl_fid; m4_dnl
m4_if(sl_breakable([[$7]]),1,[[[[$7]] _sl_brk;]],) m4_dnl
sl_pulldecls(m4_shiftn(8,$@))m4_dnl
uTC::create(_sl_fid,_sl_place,true,false,_sl_start,_sl_limit,_sl_step,_sl_block,_sl_brkref NULL,m4_dnl
&([[$8]])sl_pullargs(m4_shiftn(8,$@)))m4_dnl
]])


# Pass transparently the sync construct.
m4_define([[sl_sync]],[[m4_dnl
m4_if([[$2]],,,[[$2 = ]])uTC::sync(m4_if([[$1]],,_sl_fid,[[$1]]))m4_dnl
]])

# Pass transparently all references to argument/parameter
# names.
m4_define([[sl_geta]],[[__a_[[$1]]]])
m4_define([[sl_seta]],[[__a_[[$1]] = $2]])
m4_define([[sl_getp]],[[__p_[[$1]]]])
m4_define([[sl_setp]],[[__p_[[$1]] = [[$2]]]])

# Pass transparently break and kill
m4_define([[sl_break]],[[uTC::Break($1)]])
m4_define([[sl_kill]],[[uTC::kill($1)]])

# Pass transparently the family id
m4_define([[sl_getfid]],[[$1]])

# Pass transparently the break id
m4_define([[sl_getbr]],[[$1]]_brk)

