// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void)
{ }
ut_enddef

ut_decl((*foo1), void) = &foo;

ut_def(t_main, void)
{
  ut_decl((*foo2), void) = &foo;

  ut_create(,,,,,,, *foo1);
  ut_sync();

  ut_create(,,,,,,, *foo2);
  ut_sync();
}
ut_enddef

