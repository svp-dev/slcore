#include <libutc.h>
m4_include(svp/iomacros.slh)

ut_def(foo, void, ut_shparm(int, a)) 
{
   ut_setp(a, ut_getp(a) + 1);

   putc('.');
} 
ut_enddef

ut_def(t_main, void) 
{
  ut_create(,, 0, 10, 1, 0,,
            foo, ut_sharg(int, x));
  ut_seta(x, 0);
  ut_sync();

  printf1("\n%d\n", ut_geta(x));
} 
ut_enddef
