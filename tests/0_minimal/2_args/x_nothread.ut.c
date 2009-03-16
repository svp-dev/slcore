// m4_include(proto.m4)
#include <libutc.h>

void foo(int a, int b);

// XFAIL: C (not thread type)
ut_def(t_main, void)
{
  ut_create(f,,1,10,1,1,, foo, ut_glarg(int, a), ut_glarg(int, b));
  ut_sync(f);
}
ut_enddef
