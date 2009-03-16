// This is a tiny test, stolen
// from the core compiler test suite.

// m4_include(proto.m4)
#include <libutc.h>

ut_def(sum_int, void, ut_shparm(int, sum))
{
  ut_setp(sum, ut_getp(sum) + 1);
}
ut_enddef

ut_def(t_main, void)
{
  ut_create(,,0,9,1,,, sum_int, ut_sharg(int, s_in, 0));
  ut_sync();
}
ut_enddef


 
  
