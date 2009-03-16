// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, int) {} ut_enddef

// XFAIL: C (use of br out of scope)
ut_def(t_main, void)
{
  {
    ut_create(f,,,,,, int, foo);
    ut_sync(f);
  }
  int b = ut_getbr(f);
}
ut_enddef
