// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, int)
{
  ut_break(0); 
}
ut_enddef

ut_def(t_main, void)
{
  ut_create(f,,,,,, int, foo);
  ut_sync(f);
  int z = ut_getbr(f);
}
ut_enddef
