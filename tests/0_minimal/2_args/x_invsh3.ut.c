// m4_include(proto.m4)
#include <libutc.h>

// XFAIL: C (extra storage class specifier)
ut_decl(foo, void, ut_shparm(register int, a));

ut_def(bar, void, ut_shparm(register int, a)) { } ut_enddef



