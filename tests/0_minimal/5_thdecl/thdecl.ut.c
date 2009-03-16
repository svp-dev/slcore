// m4_include(proto.m4)
#include <libutc.h>

ut_decl(foo, void);
ut_decl(bar, int, ut_shparm(int, x), ut_glparm(long, y));

ut_def(t_main, void) {} ut_enddef

