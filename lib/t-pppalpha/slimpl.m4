# t-pppalpha/slimpl.m4: this file is part of the SL toolchain.
# 
# Copyright (C) 2008,2009 The SL project
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
# $Id$
#
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
m4_ifdef([[_sl_increate]],[[m4_fatal(missing sync after create)]])
m4_foreach([[__sl_parmname]],m4_quote(__sl_parmnames),[[
m4_undefine([[__sl_getp_]]__sl_parmname)
m4_undefine([[__sl_setp_]]__sl_parmname)
]])
__sl_epilogue(__sl_funcname, __sl_parmspec)
]])

# end of thread
m4_define([[sl_end_thread]], [[goto __sl_end_thread]])


m4_define([[sl_getp]],[[__sl_getp_$1]])
m4_define([[sl_setp]],[[__sl_setp_$1([[$2]])]])

m4_define([[sl_sharg]],[[sh:i:[[$1]]:[[$2]]:[[$3]]]])
m4_define([[sl_glarg]],[[gl:i:[[$1]]:[[$2]]:[[$3]]]])
m4_define([[sl_shfarg]],[[sh:f:[[$1]]:[[$2]]:[[$3]]]])
m4_define([[sl_glfarg]],[[gl:f:[[$1]]:[[$2]]:[[$3]]]])

m4_define([[sl_geta]],[[__sl_geta_$1]])
m4_define([[sl_seta]],[[__sl_seta_$1([[$2]])]])

m4_define([[m4_sh_escape]],[['m4_bpatsubst([[$1]],[[[']]],[['"'"']])']])

m4_define([[sl_initarg]],[[ m4_dnl
[[$3]] __sl_after_[[$4]]; m4_dnl
m4_define([[__sl_geta_$4]],__sl_after_$4) m4_dnl
m4_define([[__sl_seta_$4]],__sl_after_$4 = $[[]]1) m4_dnl
]])


m4_define([[sl_create]], [[m4_dnl
m4_ifdef([[_sl_increate]],[[m4_fatal(cannot nest create)]])m4_dnl
m4_define([[_sl_increate]],1)m4_dnl
m4_step([[__sl_crcnt]])m4_dnl
m4_define([[__sl_tag]],__child[[]]__sl_crcnt)m4_dnl
m4_define([[_sl_place]],m4_if([[$2]],,PLACE_DEFAULT,[[$2]]))m4_dnl
m4_define([[_sl_start]],m4_if([[$3]],,0,[[$3]]))m4_dnl
m4_define([[_sl_limit]],m4_if([[$4]],,1,[[$4]]))m4_dnl
m4_define([[_sl_step]],m4_if([[$5]],,1,[[$5]]))m4_dnl
m4_define([[_sl_block]],m4_if([[$6]],,0,[[$6]]))m4_dnl
m4_define([[__sl_crbrktype]],"[[$7]]")m4_dnl
m4_define([[__sl_crfuncname]],[[$8]])m4_dnl
m4_define([[_sl_thargs]],m4_dquote(m4_shiftn(8,$@)))m4_dnl
m4_foreach([[_sl_arg]],m4_quote(_sl_thargs),[[m4_apply([[sl_initarg]],m4_split(_sl_arg,:))]]) m4_dnl
m4_define([[__sl_thargs2]], m4_mapall_sep([[m4_sh_escape]],[[ ]], m4_dquote(m4_shiftn(8, $@))))
m4_esyscmd(m4_quote(PYTHON SPP_PY pppalpha create __sl_crfuncname __sl_tag __sl_crbrktype __sl_thargs2))
m4_assert(m4_sysval == 0)
m4_if([[$1]],,,[[([[$1]]) = __sl_fid_[[]]__sl_tag;]])
]])

m4_define([[__sl_makewreg]],[["=rf"([[$1]])]])
m4_define([[__sl_makerreg]],[["rf"([[$1]])]])

m4_define([[sl_copyarg]],[[m4_dnl
__sl_after_[[$4]] = __sl_arg_[[$4]]; m4_dnl
m4_popdef([[__sl_geta_$4]]) m4_dnl
m4_popdef([[__sl_seta_$4]]) m4_dnl
]])

m4_define([[sl_sync]],[[ m4_dnl
m4_ifndef([[_sl_increate]],[[m4_fatal(sync without create)]]) m4_dnl
m4_if([[$1]],,[[ m4_dnl
m4_define([[__sl_syncsrc]],[[%__sl_nrwrites]]) m4_dnl
m4_define([[__sl_syncdst]],[[$[[]]31]]) m4_dnl
]],[[ m4_dnl
m4_define([[__sl_wqueue_sync]], m4_join([[,]], m4_quote(__sl_wqueue_sync), [["=r"([[$1]])]])) m4_dnl
m4_define([[__sl_syncsrc]],[[%m4_incr(__sl_nrwrites)]]) m4_dnl
m4_define([[__sl_syncdst]],[[%__sl_nrwrites]]) m4_dnl
]]) m4_dnl
m4_define([[__sl_rqueue_sync]], 
m4_join([[,]], [["r"(__sl_sync_[[]]__sl_tag), "r"(__sl_arg___fptr[[]]__sl_tag)]], __sl_rqueue_sync)) m4_dnl
__asm__ __volatile__("bis __sl_syncsrc, $[[]]31, __sl_syncdst # SYNC __sl_tag" m4_dnl
: __sl_wqueue_sync m4_dnl
: __sl_rqueue_sync : "memory"); m4_dnl
m4_foreach([[_sl_arg]],m4_quote(_sl_thargs),[[m4_apply([[sl_copyarg]],m4_split(_sl_arg,:))]]) m4_dnl
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

m4_define([[sl_farg]], [[[[$1]]:[[$2]]]])

m4_define([[sl_funcall]],[[m4_dnl
m4_set_contains([[sl_inline_functions]],[[$3]],[[[[$3]](m4_shift3($@))]],[[m4_dnl
m4_pushdef([[fcall_args]],m4_dquote(m4_shift3($@)))m4_dnl
m4_pushdef([[fcall_argcount]],0)m4_dnl
({
	  struct {
            long protocol;
	    void *fptr;
	    union { long v; unsigned long uv; void *p; } retval;
	    double fretval;
            m4_foreach([[_sl_farg]],m4_quote(fcall_args),[[
union { long __pad__; m4_car(m4_unquote(m4_split(_sl_farg,:))) v; } arg[[]]fcall_argcount;
m4_step([[fcall_argcount]])
]])
} __sl_fargs;
__sl_fargs.protocol = fcall_argcount;
__sl_fargs.fptr = ([[$3]]);
m4_popdef([[fcall_argcount]])m4_dnl
m4_pushdef([[fcall_argcount]],0)m4_dnl
            m4_foreach([[_sl_farg]],m4_quote(fcall_args),[[
__sl_fargs.arg[[]]fcall_argcount.v = (m4_unquote(m4_cdr(m4_unquote(m4_split(_sl_farg,:)))));
m4_step([[fcall_argcount]])
]])
m4_popdef([[fcall_argcount]])m4_dnl
m4_popdef([[fcall_args]])m4_dnl
sl_create(,[[$1]],(long)(void*)&__sl_fargs,((long)(void*)&__sl_fargs)+1,,,, _sl_callgate);
sl_sync();
m4_case([[$2]],
        [[void]],[[]],
        [[double]],[[__sl_fargs.fretval;]],
        [[long]],[[__sl_fargs.retval.v;]],
        [[ulong]],[[__sl_fargs.retval.uv;]],
        [[ptr]],[[(void*)__sl_fargs.retval.p;]],
        [[m4_fatal([[unknown return type in funcall ("$2", not one of void/double/long/ptr)]])]])
})]])m4_dnl
]])




