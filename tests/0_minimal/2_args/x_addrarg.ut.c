// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void, ut_shparm(int, x)) {} ut_enddef

// XFAIL: C (shared has no address)
ut_def(t_main, void)
{
  int *a;
  ut_create(f1,,,,,,,foo, ut_sharg(int, b, 0));
  a = &(ut_geta(b));
  ut_sync(f1);
}
ut_enddef
