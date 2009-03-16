// m4_include(proto.m4)
#include <libutc.h>

//test break with some control flow

ut_def(foo, int, 
	ut_glparm(int, i),ut_glparm(int, b))
{

  int x=ut_getp(i);
  unsigned long long int y=ut_getp(b); //test for DImode

  if(y){
    ut_break(x);
  }else
    ut_break(y);

}
ut_enddef

ut_def(t_main, void)
{
  // do nothing
}
ut_enddef
