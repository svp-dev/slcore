// m4_include(proto.m4)
#include <libutc.h>

struct { int a; } bogus;

ut_def(foo, void) {} ut_enddef

// XFAIL: C (invalid type for exit code)
ut_def(t_main, void)
{
  ut_create(,,,,,,, foo);
  ut_sync(, bogus);
}
ut_enddef


 
