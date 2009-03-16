// m4_include(proto.m4)
#include <libutc.h>

// XFAIL: C (incompatible break type)
ut_def(foo, float)
{
  ut_break("hello");
}
ut_enddef
