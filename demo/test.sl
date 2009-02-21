#include <libutc.h>
m4_include(svp/iomacros.slh)

ut_def(inc, int,
       ut_shparm(int, x))
{
  ut_index(i);
  ut_setp(x, ut_getp(x) + 1);
  if ((int)i > 10)
    ut_break((int)i);
}
ut_enddef

ut_decl((*inc2), int, ut_shparm(int, x)) = inc;

ut_def(inc3, void,
       ut_shparm(int, x),
       ut_glparm(int, y))
{ 
  ut_setp(x, ut_getp(x) + ut_getp(y));
}
ut_enddef


ut_def(t_main, void)
{
  int r;

  {
    puts("Test #1:\n");
    ut_create(f1,,1,20,2,3, void,
	      inc3, ut_sharg(int, n, 0), ut_glarg(int, g, 1));
    ut_sync(f1);
    printf1("output count: %d\n", ut_geta(n));
  }
  
  {
    puts("Test #2:\n");
    family cf;
    ut_create(f,,1,10,2,3,,
	      inc3, ut_sharg(int, n, 0), ut_glarg(int, incr, 1));
    cf = ut_getfid(f);
    ut_kill(cf);
    ut_sync(f,r);

    if (r == EXIT_KILLED) 
      puts("killed!\n");
    else {
      printf1("counter: %d\n", ut_geta(n));
    }
  }

    
  {
    puts("Test #3:\n");
    ut_create(,PLACE_LOCAL,,,,, void,
	      inc3, ut_sharg(int, n2), ut_glarg(int, i, 2));
    ut_seta(n2, 0);
    ut_sync();
    puts("done.\n");
  }

  {
    puts("Test #4:\n");
    ut_create(fx,,1,20,2,3, int,
	      inc, ut_sharg(int, n, 0));
    ut_sync(fx,r);
    if (r == EXIT_BROKEN) {
      printf1("broken, brk = %d\n", ut_getbr(fx));
    } else {
      printf1("output count: %d\n", ut_geta(n));
    }
  }
}
ut_enddef

