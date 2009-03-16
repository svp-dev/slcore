// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void, ut_glparm(int, a))
{ }
ut_enddef

// XFAIL: C (global is immutable)
ut_def(t_main, void)
{
  ut_create(,,,,,,, foo, ut_glarg(int, a, 10));
  ut_seta(a, 10);
  ut_sync();
}
ut_enddef 
  
