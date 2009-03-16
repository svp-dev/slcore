// m4_include(proto.m4)
#include <libutc.h>
#include <utcdebug.h>

// XFAIL: C (index not at beginning)
ut_def(foo, void)
{
  int x;
  ut_index(i);
}
ut_enddef

ut_def(bar, void)
{
  d_nop();
  ut_index(i);
}
ut_enddef




