// m4_include(proto.m4)
#include <libutc.h>
#include <utcdebug.h>

ut_def(foo, void) 
{
  d_nop();
}
ut_enddef

// XTIMEOUT: R
ut_def(t_main, void)
{
  ut_create(,,0,1,0,,, foo);
  ut_sync();
}
ut_enddef
