// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, int)
{
  ut_break(0); 
}
ut_enddef

// XFAIL: C (break is immutable)
ut_def(t_main, void)
{
  ut_create(f,,,,,, int, foo);
  ut_sync(f);
  ut_getbr(f) = 10;
}
ut_enddef
