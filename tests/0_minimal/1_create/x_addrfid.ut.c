// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void) {} ut_enddef

// XFAIL: C (fid has no address)
ut_def(t_main, void)
{
  family *f;
  ut_create(f1,,,,,,,foo);
  f = &(ut_getfid(f1));
  ut_sync(f1);
}
ut_enddef
