// This is a tiny test, stolen
// from the core compiler test suite (swch_cond.c)
// m4_include(proto.m4)
#include <libutc.h>

int result[10];

ut_def(t_main, void)
{
  int i;
  i = 1;
  if (i) 
    result[0] = 2;
  else
    result[0] = 3;
}
ut_enddef

