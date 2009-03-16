// m4_include(proto.m4)
#include <libutc.h>
#include <utcdebug.h>

ut_def(func, void)
{
  d_puts("hello, world\n");
}
ut_enddef

// XFAIL: C (create without sync should fail during parsing)

ut_def(t_main, void)
{
  ut_create(f,,0,1,1,1,, func);
}
ut_enddef
