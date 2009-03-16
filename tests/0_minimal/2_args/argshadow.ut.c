// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void, ut_glparm(int, a)) {} ut_enddef

struct { int x; } a;
ut_def(t_main, void)
{
  if (1) {
    ut_create(f,,,,,,,foo, ut_glarg(int, a, 10)); // valid: different scope
    ut_sync(f);
    int z = ut_geta(a); // a still visible
  }
  a.x = 10; // previous a now visible
}
ut_enddef
