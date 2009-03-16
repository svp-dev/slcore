// m4_include(proto.m4)
#include <libutc.h>

//test break with some control flow

ut_def(foo, int,
        ut_glparm(int, i))
{

  int x=ut_getp(i);

  if(x){
    ut_break(254);
  }else
    ut_break(255);

}
ut_enddef

ut_def(t_main, void)
{
  // do nothing
}
ut_enddef
