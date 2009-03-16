// m4_include(proto.m4)
#include <libutc.h>
#include <utcdebug.h>

ut_def(foo, void) { } ut_enddef

ut_def(t_main, void)
{
  int r;
  ut_create(f,,,,,, void, foo);
  ut_kill(ut_getfid(f));
  ut_sync(f, r);
  if (r == EXIT_KILLED) 
    d_nop();
}
ut_enddef
