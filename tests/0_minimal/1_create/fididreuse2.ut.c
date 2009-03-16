// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void) {} ut_enddef

ut_def(t_main, void)
{
  ut_create(f,,1,10,2,3,, foo);
  ut_sync(f);

  float f;
  f = 3. + 14.;

  ut_create(f,,1,10,2,3,, foo);
  // valid: new scope
  ut_sync(f);
}
ut_enddef
