// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void, ut_glparm(int, a)) 
{ }
ut_enddef

ut_def(t_main, void)
{
  ut_create(,,,,,,, foo, ut_glarg(int, x, 10));
  ut_sync();
  int z = ut_geta(x);
}
ut_enddef
