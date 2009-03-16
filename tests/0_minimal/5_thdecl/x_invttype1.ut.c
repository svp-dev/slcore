// m4_include(proto.m4)
#include <libutc.h>

ut_decl(foo, int, ut_shparm(int, a), ut_glparm(int, b));

// XFAIL: C (incompatible break type)
ut_decl((*foop), float, ut_shparm(int, a), ut_glparm(int, b)) = &foo;
