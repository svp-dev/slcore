// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void) {} ut_enddef

ut_def(t_main, void)
{
  ut_create(f,,1,10,2,3,, foo);
  ut_sync(f);

  ut_create(f2,,23,10,-3,3,, foo);
  ut_sync(f2);
}
ut_enddef
