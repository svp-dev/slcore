// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void) {} ut_enddef

// XFAIL: C (create not statement)
ut_def(t_main, void)
{
  if (1) 
    ut_create(f,,1,10,2,3,, foo);
    ut_sync(f);
  
}
ut_enddef
