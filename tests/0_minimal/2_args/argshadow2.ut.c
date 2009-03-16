// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void, ut_glparm(int, a)) {} ut_enddef

ut_def(t_main, void)
{
  ut_create(f,,,,,,,foo, ut_glarg(int, a, 10)); 
  float a = 3.14; // valid: different scope
  ut_sync(f);
}
ut_enddef
