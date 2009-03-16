// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void, ut_glparm(int, a)) {} ut_enddef
ut_def(bar, void, ut_shparm(int, a)) {} ut_enddef

ut_def(t_main, void)
{
  ut_create(f1,,1,10,1,1,, foo, ut_sharg(int, , 10));
  ut_sync(f1);

  ut_create(f2,1,10,1,1,, bar, ut_glarg(int, , 20));
  ut_sync(f2);
}
ut_enddef

