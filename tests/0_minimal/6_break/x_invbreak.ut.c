// m4_include(proto.m4)
#include <libutc.h>

// XFAIL: C (thread is not breakable)
ut_def(foo, void)
{
  ut_break(10);
}
ut_enddef
