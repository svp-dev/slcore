// m4_include(proto.m4)
#include <libutc.h>

ut_decl(foo, int, ut_shparm(int, a), ut_glparm(int, b));

// XFAIL: C (invalid use of ellipsis)
ut_decl((*foop), int, ut_shparm(int, a), ut_glparm(int, b), ...) = &foo;
