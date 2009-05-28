m4_init()
# slc.m4: this file is part of the slc project.
#
# Copyright (C) 2008,2009 The SL project.
# All rights reserved.
#
# $Id$

# m4_incr: helper macro
m4_define([[m4_step]],[[m4_define([[$1]], m4_incr($1))]])

# sl_breakable() - helper macro, returns 1 if type is non-void or
# non-empty
m4_define([[sl_breakable]],[[m4_case([[$1]],[[void]],0,[[]],0,1)]])

# restrict: enable C99 keyword
m4_define([[restrict]], [[__restrict__]])

# load implementation-specific definitions
m4_include([[slimpl.m4]])

# sl_proccall() - helper macro, encapsulate a singleton create
m4_define([[sl_proccall]],[[do { sl_create(,,,,,,,$@); sl_sync(); } while(0)]])

# sl_begin_header() / sl_end_header() - helper macros, protect against multiple inclusion
m4_define([[sl_begin_header]],[[m4_dnl
m4_ifndef([[$1_found]],[[m4_define([[$1_found]],1)m4_divert_push(0)]],[[m4_divert_push([[KILL]])]])m4_dnl
]])

m4_define([[sl_end_header]],[[m4_dnl
m4_divert_pop()
]])

# Initialize C mode: disable m4 comments, 
# stop diverting.
m4_changecom(//)
m4_wrap_lifo([[m4_divert_pop(0)]])
m4_divert_push(0)

#include "sl_support.h"
