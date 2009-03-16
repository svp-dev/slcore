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

ut_def(foo, void)
{
  ut_create(,,0,9,1,1,, nopfunc);
  ut_sync();
}
ut_enddef

ut_def(t_main, void)
{
  ut_create(,,0,9,1,1,, foo);
  ut_sync();
}
ut_enddef


 
  
