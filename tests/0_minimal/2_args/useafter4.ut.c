// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void, ut_shparm(int, a), ut_glparm(int, b)) 
{ }
ut_enddef

ut_def(t_main, void)
{
  ut_create(,,,,,,, foo, ut_sharg(int, x), ut_glarg(int, y, 10));
  ut_seta(x, 20);
  ut_sync();
  int z = ut_geta(x) + ut_geta(y);
}
ut_enddef
