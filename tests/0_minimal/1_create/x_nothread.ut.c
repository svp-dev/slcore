// m4_include(proto.m4)
#include <libutc.h>

// XFAIL: C (not thread type)
ut_def(t_main, void)
{
  ut_create(f,,1,10,1,1,, (40+2));
  ut_sync(f);
}
ut_enddef
