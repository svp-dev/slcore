// m4_include(proto.m4)
#include <libutc.h>
#include <utcdebug.h>

ut_def(foo, void, ut_shparm(int, a))
{
  ut_setp(a, ut_getp(a) + 1);
}
ut_enddef

ut_def(t_main, void)
{
  int busy;
  ut_create(,,,,,,,
	    foo, ut_sharg(int, a));
  for (busy = 0; busy < 1000; ++busy) d_putchar('.');
  ut_seta(a, 42);
  ut_sync();

  d_putchar('\n');
  d_put_signed(ut_geta(a), int, 10);
  d_putchar('\n');
}
ut_enddef
