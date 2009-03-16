// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void) {} ut_enddef

ut_def(t_main, void)
{
  ut_create(f,,1,10,2,3,, foo);
  ut_sync(f);

  ut_create(f,,1,10,2,3,, foo);
  // valid: new scope
  ut_sync(f);

  ut_create(f,,1,10,2,3,, foo);
  ut_kill(ut_getfid(f)); // valid: f defined until sync
  ut_sync(f);

}
ut_enddef
