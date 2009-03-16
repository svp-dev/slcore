// This is a tiny test, stolen
// from the core compiler test suite.

// m4_include(proto.m4)
#include <libutc.h>
#include <utcdebug.h>

ut_def(nopfunc, void)
{
  d_nop();
}
ut_enddef

ut_def(t_main, void)
{
  ut_create(,,,,,,, nopfunc);
  ut_sync();
}
ut_enddef


 
  
