// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void) {} ut_enddef

struct { int a; } bogus; // not a place

// XFAIL: C (invalid type for place)
ut_def(t_main, void)
{
  ut_create(f, bogus,1,10,2,3,, foo);
  ut_sync(f);
}
ut_enddef
