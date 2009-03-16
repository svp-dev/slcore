// m4_include(proto.m4)
#include <libutc.h>

ut_def(bar, char*, ut_glparm(char*, a)) {} ut_enddef

ut_decl((*barp), void*, ut_glparm(char*, a)) = &bar; 

ut_def(t_main, void) { } ut_enddef
