// m4_include(proto.m4)
#include <libutc.h>

// XFAIL: C (global has no address)
int *p;
ut_def(foo, void, ut_glparm(int, a))
{
  p = &(ut_getp(a));
  ut_setp(a, ut_getp(a));
}
ut_enddef
