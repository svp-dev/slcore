// m4_include(proto.m4)
#include <libutc.h>
#include <utcdebug.h>

ut_def(foo, void, ut_shparm(int, a)) {
   ut_setp(a, ut_getp(a) + 1);
   d_putchar('.');
} ut_enddef

ut_def(t_main, void) {
  ut_create(,, 0, 10, 1, 0,,
            foo, ut_sharg(int, x));
  ut_seta(x, 0);
  ut_sync();
  d_putchar('\n');
  d_put_signed(ut_geta(x), d_divmods32, int, 10); d_putchar('\n');
} ut_enddef
