#include <libutc.h>
sl_include(svp/iomacros.slh)

sl_def(inc, int,
       sl_shparm(int, x))
{
  sl_index(i);
  sl_setp(x, sl_getp(x) + 1);
  if ((int)i > 10)
    sl_break((int)i);
}
sl_enddef

sl_decl((*inc2), int, sl_shparm(int, x)) = inc;

sl_def(inc3, void,
       sl_shparm(int, x),
       sl_glparm(int, y))
{ 
  sl_setp(x, sl_getp(x) + sl_getp(y));
}
sl_enddef


sl_def(t_main, void)
{
  int r;

  {
    puts("Test #1:\n");
    sl_create(f1,,1,20,2,3, void,
	      inc3, sl_sharg(int, n, 0), sl_glarg(int, g, 1));
    sl_sync(f1);
    printf1("output count: %d\n", sl_geta(n));
  }
  
  {
    puts("Test #2:\n");
    family cf;
    sl_create(f,,1,10,2,3,,
	      inc3, sl_sharg(int, n, 0), sl_glarg(int, incr, 1));
    cf = sl_getfid(f);
    sl_kill(cf);
    sl_sync(f,r);

    if (r == EXIT_KILLED) 
      puts("killed!\n");
    else {
      printf1("counter: %d\n", sl_geta(n));
    }
  }

    
  {
    puts("Test #3:\n");
    sl_create(,PLACE_LOCAL,,,,, void,
	      inc3, sl_sharg(int, n2), sl_glarg(int, i, 2));
    sl_seta(n2, 0);
    sl_sync();
    puts("done.\n");
  }

  {
    puts("Test #4:\n");
    sl_create(fx,,1,20,2,3, int,
	      inc, sl_sharg(int, n, 0));
    sl_sync(fx,r);
    if (r == EXIT_BROKEN) {
      printf1("broken, brk = %d\n", sl_getbr(fx));
    } else {
      printf1("output count: %d\n", sl_geta(n));
    }
  }
}
sl_enddef

