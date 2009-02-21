# ###############################################
#  Macro definitions for the new muTC syntax
# ###############################################

# Pass transparently thread definitions.
m4_define([[ut_def]],[[m4_dnl
m4_define([[ut_thparms]],[[m4_shiftn(2,$@)]])m4_dnl
thread [[$2]] [[$1]]m4_ifelse((ut_thparms),(),(void),(ut_thparms))m4_dnl
]])

# No special action at the end of a definition
m4_define([[ut_enddef]],[[]])

# With the new syntax, a declaration looks the same as a definition.
m4_define([[ut_decl]], m4_defn([[ut_def]]))

# Pass transparently parameter declarations.
m4_define([[ut_shparm]], [[shared [[$1]] [[$2]]]])
m4_define([[ut_glparm]], [[/*global*/ [[$1]] [[$2]]]])

# Pass transparently the index declaration.
m4_define([[ut_index]], [[index [[$1]]]])

# Pass transparently the create statement; auto-generate
# break variable names.
m4_define([[ut_create]], [[m4_dnl
m4_define([[_ut_brk]],m4_ifelse(ut_breakable([[$7]]),1,[[[[$1]]_brk]],))m4_dnl
create([[$1]];[[$2]];[[$3]];[[$4]];[[$5]];[[$6]];_ut_brk)m4_dnl
 [[$8]](m4_shiftn(8,$@)) m4_dnl
]])


# Pass transparently shared and global argument declarations.
m4_define([[ut_sharg]],[[m4_dnl
[[$2]]m4_ifelse([[$3]],,,[[ = $3]]) m4_dnl
]])
m4_define([[ut_glarg]],m4_defn([[ut_sharg]]))

# Pass transparently the sync construct.
m4_define([[ut_sync]],[[m4_dnl
m4_ifelse([[$2]],,,[[$2 = ]])sync([[$1]])m4_dnl
]])

# Pass transparently all references to argument/parameter
# names.
m4_define([[ut_geta]],[[$1]])
m4_define([[ut_seta]],[[$1 = $2]])
m4_define([[ut_getp]],[[$1]])
m4_define([[ut_setp]],[[$1 = $2]])

# Pass transparently break and kill
m4_define([[ut_break]],[[break ($1)]])
m4_define([[ut_kill]],[[kill($1)]])

# Pass transparently ut_getfid
m4_define([[ut_getfid]],[[$1]])

# Pass transparently ut_getbr
m4_define([[ut_getbr]],[[$1]]_brk)

# ## End macros for new muTC syntax ###
