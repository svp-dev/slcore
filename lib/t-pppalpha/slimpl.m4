###############################################
# Macro definitions for the PPP-Alpha compiler
###############################################

m4_define([[__sl_dispcount]],0)
m4_define([[__sl_crcnt]],0)

m4_define([[sl_decl]], [[void [[$1]](void)]])

m4_define([[sl_shparm]],[[sh:i:[[$1]]:[[$2]]]])
m4_define([[sl_glparm]],[[gl:i:[[$1]]:[[$2]]]])
m4_define([[sl_shfparm]],[[sh:f:[[$1]]:[[$2]]]])
m4_define([[sl_glfparm]],[[gl:f:[[$1]]:[[$2]]]])

m4_define([[sl_def]],[[
m4_define([[__sl_funcname]], [[$1]])
m4_define([[__sl_breaktype]], [[$2]])
m4_step([[__sl_dispcount]])
m4_define([[__sl_thparms]],"m4_join([[" "]],m4_shiftn(2,$@))")
m4_esyscmd(m4_quote(PYTHON SPP_PY pppalpha fundef __sl_funcname __sl_dispcount __sl_thparms))
m4_assert(m4_sysval == 0)
]])
m4_define([[sl_enddef]],[[
m4_foreach([[__sl_parmname]],m4_quote(__sl_parmnames),[[
m4_undefine([[__sl_getp_]]__sl_parmname)
m4_undefine([[__sl_setp_]]__sl_parmname)
]])
__sl_epilogue(__sl_funcname, __sl_parmspec)
]])

m4_define([[sl_getp]],[[__sl_getp_$1]])
m4_define([[sl_setp]],[[__sl_setp_$1([[$2]])]])

m4_define([[sl_sharg]],[[sh:i:[[$1]]:[[$2]]:[[$3]]]])
m4_define([[sl_glarg]],[[gl:i:[[$1]]:[[$2]]:[[$3]]]])
m4_define([[sl_shfarg]],[[sh:f:[[$1]]:[[$2]]:[[$3]]]])
m4_define([[sl_glfarg]],[[gl:f:[[$1]]:[[$2]]:[[$3]]]])

m4_define([[sl_geta]],[[__sl_geta_$1]])
m4_define([[sl_seta]],[[__sl_seta_$1([[$2]])]])

m4_define([[m4_sh_escape]],[['m4_bpatsubst([[$1]],[[[']]],[['"'"']])']])

m4_define([[sl_create]],[[
m4_step([[__sl_crcnt]])
m4_define([[__sl_autotag]],__child[[]]__sl_crcnt[[]]__)
m4_define([[__sl_tag]],m4_if([[$1]],,__sl_autotag,[[$1]]))
m4_define([[_sl_place]],m4_if([[$2]],,[[SVP_LOCAL]],[[$2]]))
m4_define([[_sl_start]],m4_if([[$3]],,0,[[$3]]))
m4_define([[_sl_limit]],m4_if([[$4]],,1,[[$4]]))
m4_define([[_sl_step]],m4_if([[$5]],,1,[[$5]]))
m4_define([[_sl_block]],m4_if([[$6]],,0,[[$6]]))
m4_define([[__sl_crbrktype]],"[[$7]]")
m4_define([[__sl_crfuncname]],"[[$8]]")
m4_define([[__sl_thargs]], m4_mapall_sep([[m4_sh_escape]],[[ ]], m4_dquote(m4_shiftn(8, $@))))
m4_esyscmd(m4_quote(PYTHON SPP_PY pppalpha create __sl_crfuncname __sl_tag __sl_crbrktype __sl_thargs))
m4_assert(m4_sysval == 0)
]])

m4_define([[__sl_makewreg]],[["=rf"([[$1]])]])
m4_define([[__sl_makerreg]],[["rf"([[$1]])]])

m4_define([[sl_sync]],[[
m4_if([[$1]],,[[
m4_define([[__sl_syncsrc]],[[%__sl_nrwrites]])
m4_define([[__sl_syncdst]],[[$[[]]31]])
]],[[
m4_define([[__sl_wqueue_sync]], m4_join([[,]], m4_quote(__sl_wqueue_sync), [["=r"([[$1]])]]))
m4_define([[__sl_syncsrc]],[[%m4_incr(__sl_nrwrites)]])
m4_define([[__sl_syncdst]],[[%__sl_nrwrites]])
]])
m4_define([[__sl_rqueue_sync]], 
m4_join([[,]], [["r"(__sl_sync_[[]]__sl_tag), "r"(__sl_glarg___fptr[[]]__sl_tag)]], __sl_rqueue_sync))
__asm__ __volatile__("bis __sl_syncsrc, $[[]]31, __sl_syncdst"
: __sl_wqueue_sync
: __sl_rqueue_sync)
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



