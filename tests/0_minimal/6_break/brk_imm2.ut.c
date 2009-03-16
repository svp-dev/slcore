// m4_include(proto.m4)
#include <libutc.h>

//test break with some control flow
int e,x;

ut_def(foo, int)
{
    x=0;

    x = x + 1;
    e = x - 1;
    ut_break(254);
    e = e - 1;
}
ut_enddef

ut_def(t_main, void)
{
  // do nothing
}
ut_enddef
