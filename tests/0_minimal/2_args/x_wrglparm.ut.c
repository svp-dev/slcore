// m4_include(proto.m4)
#include <libutc.h>

// XFAIL: C (global is immutable)
ut_def(foo, void, ut_glparm(int, a))
{
  ut_setp(a, ut_getp(a) + 1);
}
ut_enddef

 
  
