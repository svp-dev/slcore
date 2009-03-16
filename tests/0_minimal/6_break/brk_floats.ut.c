// m4_include(proto.m4)
#include <libutc.h>

//test break with some control flow

ut_def(foo, float)
{

  float  x = 10.78;

  ut_break(x);

}
ut_enddef


ut_def(foo2, double)
{

  double y = 15.294;

  ut_break(y);

}
ut_enddef


ut_def(t_main, void)
{
  // do nothing
}
ut_enddef
