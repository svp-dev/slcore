// This is a tiny test, stolen
// from the core compiler test suite (create-defaults.c)
// m4_include(proto.m4)

#include <libutc.h>

int bla;

ut_def(a, void, ut_shparm(int, x), ut_shparm(int, v))
{
  bla = ut_getp(x);
}
ut_enddef

ut_def(t_main, void)
{
  ut_create(fid,,,,,,, a, ut_sharg(int, x, 0), ut_sharg(int, v, 0));
  ut_sync(fid);
}
ut_enddef
