// m4_include(proto.m4)
#include <libutc.h>

int e;

ut_def(foo, void) {} ut_enddef

ut_def(t_main, void)
{
  ut_create(f,,,,,,, foo);
  ut_sync(f);
  if (e == 0) {
      e = e + 1;
  }
}
ut_enddef
