// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void, ut_glparm(int, x)) {} ut_enddef

// XFAIL: C (multiple declarations of x)
ut_def(t_main, void)
{
  int x;
  ut_create(,,,,,,, foo, ut_sharg(int, x, 10));
  ut_sync();
}
ut_enddef
