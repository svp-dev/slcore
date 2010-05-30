# t-seqc/slimpl.m4: this file is part of the SL toolchain.
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

m4_define([[sl_shparm]], [[[[$1]] m4_ifblank([[$2]],sl_anonymous,[[$2]])]])
m4_define([[sl_glparm]], [[[[$1]] const m4_ifblank([[$2]],sl_anonymous,[[$2]])]])
m4_define([[sl_glparm_mutable]], [[[[$1]] m4_ifblank([[$2]],sl_anonymous,[[$2]])]])
m4_copy([[sl_shparm]],[[sl_shfparm]])
m4_copy([[sl_glparm]],[[sl_glfparm]])
m4_copy([[sl_glparm_mutable]],[[sl_glfparm_mutable]])

m4_define([[sl_def]],[[m4_dnl
m4_define([[sl_thparms]],m4_dquote(m4_shift2($@)))m4_dnl
void [[$1]](struct sl_famdata * __sl_fam) { m4_dnl
struct { m4_dnl 
m4_foreach([[_sl_parm]],m4_quote(sl_thparms),[[_sl_parm;]])m4_dnl
} *__sl_args = __sl_fam->a; m4_dnl
]])

m4_define([[sl_enddef]],[[} m4_dnl
m4_ifdef([[_sl_increate]],[[m4_fatal(missing sync after create)]]) m4_dnl
]])

# end of thread
m4_define([[sl_end_thread]], [[return]])

m4_define([[sl_decl]], [[void [[$1]](struct sl_famdata * __sl_fam)]])
m4_define([[sl_decl_fptr]], [[void (*[[$1]])(struct sl_famdata *)]])

m4_define([[sl_index]], [[register const long [[$1]] = (__sl_fam->ix)]])
m4_define([[sl_getp]],[[(__sl_args->[[$1]])]])
m4_define([[sl_setp]],[[(__sl_args->[[$1]]) = [[$2]]]])


m4_define([[sl_declarg]],[[m4_dnl
[[$1]] [[$2]]; m4_dnl
]])
m4_define([[sl_copyarg]],[[m4_dnl
__after_[[$2]] = _sl_lbl[[]]_args.[[$2]]; m4_dnl
m4_popdef([[__sl_geta_$2]]) m4_dnl
m4_popdef([[__sl_seta_$2]]) m4_dnl
]])

m4_define([[sl_initarg]],[[ m4_dnl
m4_pushdef([[_sl_initializer]],m4_joinall([[:]],m4_shiftn(2,$@))) m4_dnl
m4_ifblank(m4_normalize(m4_quote(_sl_initializer)),,[[_sl_lbl[[]]_args.[[$2]] = (_sl_initializer);]]) m4_dnl
[[$1]] __after_[[$2]]; m4_dnl
m4_define([[__sl_geta_$2]],__after_$2) m4_dnl
m4_define([[__sl_seta_$2]],__after_$2 = $[[]]1) m4_dnl
m4_pushdef([[__sl_geta_$2]],_sl_lbl[[]]_args.$2)
m4_pushdef([[__sl_seta_$2]],_sl_lbl[[]]_args.$2 = $[[]]1) m4_dnl
m4_popdef([[_sl_initializer]]) m4_dnl
]])

# The following create just ignores the SVP place.
m4_define([[sl_create]], [[m4_dnl
m4_ifdef([[_sl_increate]],[[m4_fatal(cannot nest create)]])m4_dnl
m4_define([[_sl_increate]],1)m4_dnl
m4_step([[_sl_crcnt]])m4_dnl
m4_define([[_sl_lbl]],__child[[]]_sl_crcnt)m4_dnl
m4_define([[_sl_start]],m4_ifblank([[$3]],0,[[$3]]))m4_dnl
m4_define([[_sl_limit]],m4_ifblank([[$4]],1,[[$4]]))m4_dnl
m4_define([[_sl_step]],m4_ifblank([[$5]],1,[[$5]]))m4_dnl
m4_define([[_sl_thargs]],m4_dquote(m4_shiftn(8,$@)))m4_dnl
struct { m4_dnl
m4_foreach([[_sl_arg]],m4_quote(_sl_thargs),[[m4_apply([[sl_declarg]],m4_split(_sl_arg,:))]]) m4_dnl
} _sl_lbl[[]]_args; m4_dnl
m4_foreach([[_sl_arg]],m4_quote(_sl_thargs),[[m4_apply([[sl_initarg]],m4_split(_sl_arg,:))]]) m4_dnl
struct sl_famdata __slf[[]]_sl_lbl;
m4_ifblank([[$1]],,[[([[$1]]) = &__slf[[]]_sl_lbl;]]) m4_dnl
__slf[[]]_sl_lbl.be = _sl_start; m4_dnl
__slf[[]]_sl_lbl.li = _sl_limit; m4_dnl
__slf[[]]_sl_lbl.st = _sl_step; m4_dnl
__slf[[]]_sl_lbl.ex = 0; m4_dnl
__slf[[]]_sl_lbl.f = ([[$8]]); m4_dnl
__slf[[]]_sl_lbl.a = &_sl_lbl[[]]_args m4_dnl
]])


# Pass transparently shared and global argument declarations.
m4_define([[sl_sharg]],[[[[$1]]:m4_ifblank([[$2]],sl_anonymous,[[$2]]):[[$3]]]])
m4_copy([[sl_sharg]],[[sl_glarg]])
m4_copy([[sl_sharg]],[[sl_shfarg]])
m4_copy([[sl_sharg]],[[sl_glfarg]])

# Pass transparently the sync construct.
m4_define([[sl_sync]],[[m4_dnl
m4_ifndef([[_sl_increate]],[[m4_fatal(sync without create)]]) m4_dnl
m4_define([[_sl_body]],[[{ m4_dnl
__slf[[]]_sl_lbl.f(&__slf[[]]_sl_lbl); m4_dnl
if (__slf[[]]_sl_lbl.ex != 0) break; m4_dnl
} m4_dnl
]])m4_dnl
if (!__slf[[]]_sl_lbl.st)m4_dnl
 for (__slf[[]]_sl_lbl.ix = __slf[[]]_sl_lbl.be; m4_dnl
      ; __slf[[]]_sl_lbl.ix += __slf[[]]_sl_lbl.li) _sl_body m4_dnl
else if (__slf[[]]_sl_lbl.st > 0)m4_dnl
 for (__slf[[]]_sl_lbl.ix = __slf[[]]_sl_lbl.be; m4_dnl
      __slf[[]]_sl_lbl.ix < __slf[[]]_sl_lbl.li; m4_dnl
      __slf[[]]_sl_lbl.ix += __slf[[]]_sl_lbl.st) _sl_body m4_dnl
else m4_dnl
for (__slf[[]]_sl_lbl.ix = __slf[[]]_sl_lbl.be; m4_dnl
     __slf[[]]_sl_lbl.ix > __slf[[]]_sl_lbl.li; m4_dnl
     __slf[[]]_sl_lbl.ix += __slf[[]]_sl_lbl.st) _sl_body m4_dnl
m4_ifblank([[$1]],,[[[[$1]] = __slf[[]]_sl_lbl.ex;]]) m4_dnl
m4_foreach([[_sl_arg]],m4_quote(_sl_thargs),[[m4_apply([[sl_copyarg]],m4_split(_sl_arg,:))]]) m4_dnl
m4_undefine([[_sl_increate]]) m4_dnl
]])

# Pass transparently all references to argument/parameter
# names.
m4_define([[sl_geta]],[[__sl_geta_$1]])
m4_define([[sl_seta]],[[__sl_seta_$1([[$2]])]])

# Pass transparently break and kill
m4_define([[sl_break]],[[m4_dnl
do { __sl_fam->ex = 1; return; } while(0)m4_dnl
]])

m4_define([[sl_kill]],[[m4_dnl
m4_error([[kill unsupported in this implementation]])
]])

# ## End macros for new muTC syntax ###
