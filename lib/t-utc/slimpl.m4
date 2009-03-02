# ###############################################
#  Macro definitions for the new muTC syntax
# ###############################################

# Pass transparently thread definitions.
m4_define([[sl_def]],[[m4_dnl
m4_define([[sl_thparms]],[[m4_shiftn(2,$@)]])m4_dnl
thread [[$2]] [[$1]]m4_if((sl_thparms),(),(void),(sl_thparms))m4_dnl
]])

# No special action at the end of a definition
m4_define([[sl_enddef]],[[]])

# With the new syntax, a declaration looks the same as a definition.
m4_define([[sl_decl]], m4_defn([[sl_def]]))

# Pass transparently parameter declarations.
m4_define([[sl_shparm]], [[shared [[$1]] [[$2]]]])
m4_define([[sl_glparm]], [[/*global*/ [[$1]] [[$2]]]])

# Pass transparently the index declaration.
m4_define([[sl_index]], [[index [[$1]]]])

# Pass transparently the create statement; auto-generate
# break variable names.
m4_define([[sl_create]], [[m4_dnl
m4_define([[_sl_brk]],m4_if(sl_breakable([[$7]]),1,[[[[$1]]_brk]],))m4_dnl
create([[$1]];[[$2]];[[$3]];[[$4]];[[$5]];[[$6]];_sl_brk)m4_dnl
 [[$8]](m4_shiftn(8,$@)) m4_dnl
]])


# Pass transparently shared and global argument declarations.
m4_define([[sl_sharg]],[[m4_dnl
[[$2]]m4_if([[$3]],,,[[ = $3]]) m4_dnl
]])
m4_define([[sl_glarg]],m4_defn([[sl_sharg]]))

# Pass transparently the sync construct.
m4_define([[sl_sync]],[[m4_dnl
m4_if([[$2]],,,[[$2 = ]])sync([[$1]])m4_dnl
]])

# Pass transparently all references to argument/parameter
# names.
m4_define([[sl_geta]],[[$1]])
m4_define([[sl_seta]],[[$1 = $2]])
m4_define([[sl_getp]],[[$1]])
m4_define([[sl_setp]],[[$1 = $2]])

# Pass transparently break and kill
m4_define([[sl_break]],[[break ($1)]])
m4_define([[sl_kill]],[[kill($1)]])

# Pass transparently sl_getfid
m4_define([[sl_getfid]],[[$1]])

# Pass transparently sl_getbr
m4_define([[sl_getbr]],[[$1]]_brk)

# ## End macros for new muTC syntax ###
