// m4_include(proto.m4)
#include <libutc.h>

// XFAIL: C (not a thread function)
int foo(void)
{
  ut_break(10);
}
