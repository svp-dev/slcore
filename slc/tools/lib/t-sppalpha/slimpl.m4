# t-pppalpha/slimpl.m4: this file is part of the SL toolchain.
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
###############################################
# Macro definitions for the PPP-Alpha compiler
###############################################

m4_define([[__sl_dispcount]],0)
m4_define([[__sl_crcnt]],0)

m4_define([[sl_decl]], [[void [[$1]](void)]])

m4_define([[sl_decl_fptr]], [[void (*[[$1]])(void)]])

m4_define([[sl_shparm]],[[shparm:m4_normalize([[$1]]):m4_ifblank([[$2]],sl_anonymous,m4_normalize([[$2]]))]])
m4_define([[sl_glparm]],[[glparm:m4_normalize([[$1]]):m4_ifblank([[$2]],sl_anonymous,m4_normalize([[$2]]))]])
m4_define([[sl_glparm_mutable]],[[glparm_mutable:m4_normalize([[$1]]):m4_ifblank([[$2]],sl_anonymous,m4_normalize([[$2]]))]])
m4_define([[sl_shfparm]],[[shfparm:m4_normalize([[$1]]):m4_ifblank([[$2]],sl_anonymous,m4_normalize([[$2]]))]])
m4_define([[sl_glfparm]],[[glfparm:m4_normalize([[$1]]):m4_ifblank([[$2]],sl_anonymous,m4_normalize([[$2]]))]])
m4_define([[sl_glfparm_mutable]],[[glfparm_mutable:m4_normalize([[$1]]):m4_ifblank([[$2]],sl_anonymous,m4_normalize([[$2]]))]])

m4_define([[m4_sh_escape]],[['m4_bpatsubst([[$1]],[[[']]],[['"'"']])']])

m4_define([[sl_def]],[[
m4_define([[__sl_funcname]], [[$1]])
m4_define([[__sl_breaktype]], [[$2]])
m4_step([[__sl_dispcount]])
m4_define([[__sl_thparms]],"m4_join([[" "]],m4_shiftn(2,$@))")
m4_esyscmd(m4_quote(PYTHON SPP_PY sppalpha fundef m4_sh_escape(__sl_funcname) __sl_dispcount __sl_thparms))
m4_assert(m4_sysval == 0)
]])
m4_define([[sl_enddef]],[[
m4_ifdef([[_sl_increate]],[[m4_fatal(missing sync after create)]])
m4_foreach([[__sl_parmname]],m4_quote(__sl_parmnames),[[
m4_undefine([[__sl_getp_]]__sl_parmname)
m4_undefine([[__sl_setp_]]__sl_parmname)
]])
__sl_epilogue(__sl_funcname, __sl_parmspec)
]])

# end of thread
m4_define([[sl_end_thread]], [[goto __sl_end_thread]])


m4_define([[sl_getp]],[[m4_expand([[__sl_getp_]]m4_normalize([[$1]]))]])
m4_define([[sl_setp]],[[m4_expand([[__sl_setp_]]m4_normalize([[$1]])[[([[$2]])]])]])

m4_define([[sl_sharg]],[[sharg:[[$1]]:m4_ifblank([[$2]],sl_anonymous,[[$2]]):[[$3]]]])
m4_define([[sl_glarg]],[[glarg:[[$1]]:m4_ifblank([[$2]],sl_anonymous,[[$2]]):[[$3]]]])
m4_define([[sl_shfarg]],[[shfarg:[[$1]]:m4_ifblank([[$2]],sl_anonymous,[[$2]]):[[$3]]]])
m4_define([[sl_glfarg]],[[glfarg:[[$1]]:m4_ifblank([[$2]],sl_anonymous,[[$2]]):[[$3]]]])

m4_define([[sl_geta]],[[m4_expand([[__sl_geta_]]m4_normalize([[$1]]))]])
m4_define([[sl_seta]],[[m4_expand([[__sl_seta_]]m4_normalize([[$1]])[[([[$2]])]])]])

m4_define([[sl_initarg]],[[ m4_dnl
[[$2]] __sl_after_[[$3]]; m4_dnl
m4_define([[__sl_geta_$3]],[[__sl_after_[[]]m4_normalize([[$3]])]]) m4_dnl
]])

m4_define([[sl_create]], [[m4_dnl
m4_ifdef([[_sl_increate]],[[m4_fatal(cannot nest create)]])m4_dnl
m4_define([[_sl_increate]],1)m4_dnl
m4_step([[__sl_crcnt]])m4_dnl
m4_define([[__sl_tag]],__child[[]]__sl_crcnt)m4_dnl
m4_define([[_sl_place]],m4_ifblank([[$2]],0,[[$2]]))m4_dnl
m4_define([[_sl_start]],m4_ifblank([[$3]],0,[[$3]]))m4_dnl
m4_define([[_sl_limit]],m4_ifblank([[$4]],1,[[$4]]))m4_dnl
m4_define([[_sl_step]],m4_ifblank([[$5]],1,[[$5]]))m4_dnl
m4_define([[_sl_block]],m4_ifblank([[$6]],0,[[$6]]))m4_dnl
m4_define([[__sl_crbrktype]],"[[$7]]")m4_dnl
m4_define([[__sl_crfuncname]], [[$8]])m4_dnl
m4_define([[_sl_thargs]],m4_dquote(m4_shiftn(8,$@)))m4_dnl
m4_foreach([[_sl_arg]],m4_quote(_sl_thargs),[[m4_apply([[sl_initarg]],m4_split(_sl_arg,:))]]) m4_dnl
register long __sl_syncvar_[[]]__sl_tag; m4_dnl
register void* const __sl_funcptr_[[]]__sl_tag = (void*)([[$8]]);  m4_dnl
m4_define([[__sl_thargs2]], m4_mapall_sep([[m4_sh_escape]],[[ ]], m4_dquote(m4_shiftn(8, $@))))
m4_esyscmd(m4_quote(PYTHON SPP_PY sppalpha create m4_sh_escape(__sl_crfuncname) __sl_tag __sl_crbrktype __sl_thargs2))
m4_assert(m4_sysval == 0)
m4_ifblank([[$1]],,[[([[$1]]) = __sl_fid_[[]]__sl_tag;]])
]])

m4_define([[sl_sync]],[[ m4_dnl
m4_ifndef([[_sl_increate]],[[m4_fatal(sync without create)]]) m4_dnl
m4_pushdef([[__sl_syncdst]],[[$1]]) m4_dnl
__sl_atsync m4_dnl
m4_popdef([[__sl_syncdst]]) m4_dnl
m4_popdef([[__sl_atsync]]) m4_dnl
m4_undefine([[_sl_increate]]) m4_dnl
]])

m4_define([[sl_break]],[[
m4_if(__sl_breaktype,
int,[[__sl_dobreak([[$1]])]],
float,[[__sl_dobreakf([[$1]])]],
,[[
#error cannot break in this function
]])
]])

m4_define([[sl_kill]],[[__sl_dokill([[$1]])]])

m4_define([[sl_index]],[[__sl_declindex([[$1]])]])






