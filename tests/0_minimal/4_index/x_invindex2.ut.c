// m4_include(proto.m4)
#include <libutc.h>

// XFAIL: C (index not at top-level block)
ut_def(foo, void)
{
  if (1)
    ut_index(i);
}
ut_enddef




