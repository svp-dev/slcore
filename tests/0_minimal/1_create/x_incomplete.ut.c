// m4_include(proto.m4)
#include <libutc.h>

struct A;
extern struct A b;
ut_decl(foo, void, ut_glparm(struct A, a));

// XFAIL: C (thread function has incomplete type)
ut_def(t_main, void)
{
  ut_create(,,,,,,, foo, ut_glarg(struct A, a, b));
  ut_sync();
}
ut_enddef
