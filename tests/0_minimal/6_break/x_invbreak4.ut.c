// m4_include(proto.m4)
#include <libutc.h>

//create checks break type
ut_def(foo, double)
{

  double y = 15.294;

  ut_break(y);

}
ut_enddef

// XFAIL: C (break variable incorrect type)
ut_def(t_main, void)
{
  ut_create(f,,1,10,2,3,int, foo);
  ut_sync(f); 
}
ut_enddef
