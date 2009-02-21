m4_divert(-1)
m4_changequote()
m4_changequote([[,]])
m4_changecom([[#]])
# $Id$
# ###################################################
#  This is proto-muTC, a front-end for muTC programs.
# ###################################################
# 
#  This set of macros aims to support 4 modes:
#  - transform to the "core compiler syntax"
#  - transform to C++, sequential scheduling
#  - transform to the "new muTC syntax"
#  - transfrom to C++, muTC-ptl library
#
#  The following input language is defined:
# 
#  - thread definition:
#      sl_def(name, breaktype, formals...)
#      {
#           sl_index(ix);
#           ...
#      }
#      sl_enddef
# 
#    where "formals" is a comma-separated list of
#        sl_shparm(type, name)   // for shared parameters
#        sl_glparm(type, name)   // for global parameters
# 
#    sl_index() may be omitted.
#  
#  - thread declaration:
#      sl_decl(name, breaktype, args...)
# 
#  - thread family creation:
#      sl_create(lbl, place, start, limit, step, block, btype,
#                threxp, args...)
# 
#  
#    where:
#      - fid is an identifier, may be omitted if "create" (mandatory with "createb")
#      - place is an expression, may be omitted
#      - start, limit, step, block are expressions, may be omitted
#      - btype is the type of the break value (void if none)
#      - threxp is an expression
#      - args is a comma-separated list of
#          sl_sharg(type, name, init) // for shared arguments
#          sl_glarg(type, name, init) // for global arguments
#        the "init" is an initializer, may be omitted
#
#    use sl_createb for breakable threads, sl_create
#    for non-breakable threads.
# 
#  - synchronization on termination: 
#      sl_sync(lbl, exit)
#  
#    where:
#      - lbl is an identifier, may be omitted
#      - exit is an lvalue, may be omitted.
# 
#  - accessing thread arguments in the parent thread:
#      sl_geta(name)
#      sl_seta(name, value)
# 
#  - accessing thread parameters:
#      sl_getp(name)
#      sl_setp(name, value)
#
#  - accessing the break value:
#      sl_getbr(name)
#
#  - accessing the family identifier:
#      sl_getfid(lbl)
#
#  - breaking from a family:
#      sl_break(value)
#
#  - killing a family:
#      sl_kill(fid)
#
# ###########################
# Common definitions
# ###########################
#  We are preprocessing C; the standard m4 quotes are incompatible
#  with C's character constants. [[ and ]] should not appear
#  in valid C programs except in character constants, where
#  m4 substitutions are (mostly) harmless.

m4_define([[m4_shiftn]],[[m4_dnl
m4_ifelse([[$1]],0,
[[m4_shift($@)]],
[[$0(m4_eval([[$1]]-1), m4_shift(m4_shift($@)))]])m4_dnl
]])

m4_define([[sl_breakable]],[[m4_ifelse([[$1]],void,0,m4_ifelse([[$1]],,0,1))]])

m4_define([[sl_proccall]],[[do { sl_create(,,,,,,,$@); sl_sync(); } while(0)]])

m4_define([[sl_include]], m4_defn([[m4_include]]))
m4_include([[protoimpl.m4]])

m4_changecom([[m4_comment]])
m4_divert(0)m4_dnl
#include "sl_support.h"

