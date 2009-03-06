m4_init()
# $Id$
# ###########################
# Common definitions
# ###########################

# m4_incr: helper macro
m4_define([[m4_step]],[[m4_define([[$1]], m4_incr($1))]])

# sl_include() - include a header file
m4_copy([[m4_include]], [[sl_include]])

# sl_breakable() - helper macro, returns 1 if type is non-void or
# non-empty
m4_define([[sl_breakable]],[[m4_case([[$1]],[[void]],0,[[]],0,1)]])

# sl_proccall() - helper macro, encapsulate a singleton create
m4_define([[sl_proccall]],[[({ sl_create(,,,,,,,$@); sl_sync(); })]])

# load implementation-specific definitions
m4_include([[slimpl.m4]])

# Initialize C mode: disable m4 comments, 
# stop diverting.
m4_changecom(//)
m4_wrap_lifo([[m4_divert_pop(0)]])
m4_divert_push(0)

#include "sl_support.h"
