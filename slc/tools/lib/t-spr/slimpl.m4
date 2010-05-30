# t-spr/slimpl.m4: this file is part of the SL toolchain.
# 
# Copyright (C) 2008,2009,2010 The SL project
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# The complete GNU General Public Licence Notice can be found as the
# `COPYING' file in the root directory.
#
#
# ###############################################
#  Macro definitions for the sequential C syntax
# ###############################################

m4_define([[_sl_crcnt]],0)

# Thread definitions

m4_define([[_sl_doparm]], [[m4_dnl
[[{ 'loc':"""]]__file__:__line__[[""",'type':'$1', 'ctype':""" $2 """, 'name':']] m4_dnl
m4_ifblank([[$3]],sl_anonymous,[[$3]]) m4_dnl
[['}, ]] m4_dnl
]])

m4_define([[sl_glparm]], [[_sl_doparm([[glparm]],[[$1]],[[$2]])]])
m4_define([[sl_glfparm]], [[_sl_doparm([[glfparm]],[[$1]],[[$2]])]])
m4_define([[sl_shparm]], [[_sl_doparm([[shparm]],[[$1]],[[$2]])]])
m4_define([[sl_shfparm]], [[_sl_doparm([[shfparm]],[[$1]],[[$2]])]])
m4_define([[sl_glparm_mutable]], [[_sl_doparm([[glparm_mutable]],[[$1]],[[$2]])]])

m4_define([[sl_def]],[[m4_dnl
m4_ifdef([[_sl_increate]],[[m4_fatal(missing sync after create)]]) m4_dnl
m4_pushdef([[_sl_curfun]],[[$1]]) m4_dnl
[[""", #### begin fundef: $1 ####
{'loc':"""]]__file__:__line__[[""",'type':'fundef', 'name':'$1', 'params': [ ]] m4_shift2($@) [[ ], 'body':[ ### begin body:
""" ]] m4_dnl
]])

m4_define([[sl_enddef]],[[m4_dnl
[[ """ ], 'loc_end':"""]]__file__:__line__[[""" }, #### end fundef: ]] _sl_curfun [[ ####
""" ]] m4_dnl
m4_ifdef([[_sl_increate]],[[m4_fatal(missing sync after create)]]) m4_dnl
m4_popdef([[_sl_curfun]])
]])

# end of thread
m4_define([[sl_end_thread]], [[m4_dnl
[[ """, { 'loc': """]]__file__:__line__[[""", 'type' : 'end_thread' }, """ ]]m4_dnl
]])

m4_define([[sl_decl]], [[m4_dnl
m4_define([[sl_thparms]],m4_dquote(m4_shift2($@)))m4_dnl
[[""", { 'loc': """]]__file__:__line__[[""", 'type':'decl', 'name':'$1', 'params': [ ]]sl_thparms[[ ] }, """ ]] m4_dnl
]])

m4_define([[sl_index]], [[m4_dnl
[[""", { 'loc': """]]__file__:__line__[[""", 'type':'indexdecl', 'name':'$1' }, """]] m4_dnl
]])

m4_define([[sl_create]], [[m4_dnl
m4_pushdef([[_sl_increate]],1)m4_dnl
m4_step([[_sl_crcnt]])m4_dnl
m4_pushdef([[_sl_lbl]],__child[[]]_sl_crcnt)m4_dnl
[[""", ### begin create: ]] _sl_lbl [[
{ 'type':'create',
'loc':"""]]__file__:__line__[[""",
'fun':[""" $8 """],
'lbl':']]_sl_lbl[[',
'fid':[""" $1 """],
'place':[""" ]] m4_ifblank([[$2]],PLACE_DEFAULT,[[$2]]) [[ """],
'start':[""" ]] m4_ifblank([[$3]],0,[[$3]]) [[ """],
'limit':[""" ]] m4_ifblank([[$4]],1,[[$4]]) [[ """],
'step':[""" ]] m4_ifblank([[$5]],1,[[$5]]) [[ """],
'args': [ ]] m4_shiftn(8,$@) [[ ],
'body': [ """]] m4_dnl
]])

m4_define([[sl_create_excl]], [[m4_dnl
m4_pushdef([[_sl_increate]],1)m4_dnl
m4_step([[_sl_crcnt]])m4_dnl
m4_pushdef([[_sl_lbl]],__child[[]]_sl_crcnt)m4_dnl
[[""", ### begin create: ]] _sl_lbl [[
{ 'type':'create_excl',
'loc':"""]]__file__:__line__[[""",
'fun':[""" $8 """],
'lbl':']]_sl_lbl[[',
'fid':[""" $1 """],
'place':[""" ]] m4_ifblank([[$2]],PLACE_DEFAULT,[[$2]]) [[ """],
'start':[""" ]] m4_ifblank([[$3]],0,[[$3]]) [[ """],
'limit':[""" ]] m4_ifblank([[$4]],1,[[$4]]) [[ """],
'step':[""" ]] m4_ifblank([[$5]],1,[[$5]]) [[ """],
'args': [ ]] m4_shiftn(8,$@) [[ ],
'body': [ """]] m4_dnl
]])


m4_define([[_sl_doarg]], [[m4_dnl
[[{ 'type':'$1', 'ctype':'$2', 'init':[""" $4 """], 'name':']] m4_dnl
m4_ifblank([[$3]],sl_anonymous,[[$3]]) m4_dnl
[[', 'loc':"""]]__file__:__line__[["""} ]] m4_dnl
]])

m4_define([[sl_glarg]], [[_sl_doarg([[glarg]],[[$1]],[[$2]],[[$3]])]])
m4_define([[sl_glfarg]], [[_sl_doarg([[glfarg]],[[$1]],[[$2]],[[$3]])]])
m4_define([[sl_sharg]], [[_sl_doarg([[sharg]],[[$1]],[[$2]],[[$3]])]])
m4_define([[sl_shfarg]], [[_sl_doarg([[shfarg]],[[$1]],[[$2]],[[$3]])]])

# Pass transparently the sync construct.
m4_define([[sl_sync]],[[m4_dnl
m4_ifndef([[_sl_increate]],[[m4_fatal(sync without create)]]) m4_dnl
[[ """], 'loc_end':"""]]__file__:__line__[[""", 
'sync':'normal', 'result':[ """ $1 """ ] }, ### end create: ]] _sl_lbl [[
""" ]] m4_dnl
m4_popdef([[_sl_increate]]) m4_dnl
m4_popdef([[_sl_lbl]]) m4_dnl
]])

m4_define([[sl_detach]],[[m4_dnl
m4_ifndef([[_sl_increate]],[[m4_fatal(sync without create)]]) m4_dnl
[[ """], 'loc_end':"""]]__file__:__line__[[""", 'sync':'detach' }, ### end create: ]] _sl_lbl [[
""" ]] m4_dnl
m4_popdef([[_sl_increate]]) m4_dnl
m4_popdef([[_sl_lbl]]) m4_dnl
]])

m4_define([[_sl_doget]],[[m4_dnl
[[""", { 'loc':"""]]__file__:__line__[[""", 'type':'get$1', 'name':'$2' }, """ ]] m4_dnl
]])

m4_define([[_sl_doset]],[[m4_dnl
[[""", { 'loc':"""]]__file__:__line__[[""", 'type':'set$1', 'name':'$2', 'body': [""" $3 """] }, """ ]] m4_dnl
]])

# Pass transparently all references to argument/parameter
# names.
m4_define([[sl_geta]],[[_sl_doget([[a]], [[$1]])]])
m4_define([[sl_getp]],[[_sl_doget([[p]], [[$1]])]])
m4_define([[sl_seta]],[[_sl_doset([[a]], [[$1]], [[$2]])]])
m4_define([[sl_setp]],[[_sl_doset([[p]], [[$1]], [[$2]])]])

# pass transparently function call
m4_define([[sl_funcall]], [[[[$3]](m4_shift3($@))]])



