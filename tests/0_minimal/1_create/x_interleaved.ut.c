// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void) {} ut_enddef

// XFAIL: C (invalid scoping)
ut_def(t_main, void)
{
  ut_create(f1,,,,,,, foo);
  ut_create(f2,,,,,,, foo);
  ut_sync(f1);
  ut_sync(f2);
}
ut_enddef
