#include <libutc.h>
m4_include(svp/iomacros.slh)

#define number unsigned long long

ut_def(fibo_compute, void, 
       ut_shparm(number, prev), ut_shparm(number, prev2), ut_glparm(number*, fibo))
{
  ut_index(i);

  number n = ut_getp(prev) + ut_getp(prev2);
  ut_setp(prev2, ut_getp(prev));
  ut_setp(prev, n);
  ut_getp(fibo)[i] = n;
}
ut_enddef

ut_def(fibo_print, void,
       ut_shparm(number, guard), ut_glparm(number*, fibo))
{
  ut_index(i);

  number p1 = ut_getp(fibo)[i - 2];
  number p2 = ut_getp(fibo)[i - 1];
  number p3 = ut_getp(fibo)[i];

  number n = ut_getp(guard);
  printf4("The %uth Fibonacci number is %u + %u = %u\n", (unsigned)i, p1, p2, p3);
  ut_setp(guard, n);
}
ut_enddef

#define N 45
number fibonums[N];

ut_def(t_main, void)
{
  // first, compute the numbers.
  fibonums[0] = fibonums[1] = 1;
  ut_create(fid,,2,N,,,, 
	    fibo_compute, 
	    ut_sharg(number, prev, 1), 
	    ut_sharg(number, prev2, 1),
	    ut_glarg(number*, fibo, fibonums));
  ut_sync(fid);

  // then, print them.
  ut_create(,,2,N,,,,
	    fibo_print, 
	    ut_sharg(number, guard, 0), ut_glarg(number*, t, fibonums));
  ut_sync();
  
}
ut_enddef
