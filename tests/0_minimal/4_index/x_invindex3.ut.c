// m4_include(proto.m4)
#include <libutc.h>

// XFAIL: C (index not at top-level block)
ut_def(bar, void)
{
  do {
    ut_index(i);
  } while(0);
}
ut_enddef




