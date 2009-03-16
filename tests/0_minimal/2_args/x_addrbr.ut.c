// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, int) {} ut_enddef

// XFAIL: C (breakv has no address)
ut_def(t_main, void)
{
  int *a;
  ut_create(f1,,,,,,int,foo);
  ut_sync(f1);
  a = &(ut_getbr(f1));
}
ut_enddef
