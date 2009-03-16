// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, int) {} ut_enddef

// XFAIL: C (use of br before it becomes visible)
ut_def(t_main, void)
{
  int b;
  ut_create(f,,,,,, int, foo);
  b = ut_getbr(f);
  ut_sync(f);
}
ut_enddef
