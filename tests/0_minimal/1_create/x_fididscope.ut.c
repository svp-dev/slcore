// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void) {} ut_enddef

// XFAIL: C (use of fid out of scope)
ut_def(t_main, void)
{
  ut_create(f,,,,,,, foo);
  ut_sync(f);
  family x = ut_getfid(f);
}
ut_enddef
