#include <libutc.h>
sl_include(svp/iomacros.slh)

#define number unsigned long long

sl_def(fibo_compute, void, 
       sl_shparm(number, prev), sl_shparm(number, prev2), sl_glparm(number*, fibo))
{
  sl_index(i);

  number n = sl_getp(prev) + sl_getp(prev2);
  sl_setp(prev2, sl_getp(prev));
  sl_setp(prev, n);
  sl_getp(fibo)[i] = n;
}
sl_enddef

sl_def(fibo_print, void,
       sl_shparm(number, guard), sl_glparm(number*, fibo))
{
  sl_index(i);

  number p1 = sl_getp(fibo)[i - 2];
  number p2 = sl_getp(fibo)[i - 1];
  number p3 = sl_getp(fibo)[i];

  number n = sl_getp(guard);
  printf4("The %uth Fibonacci number is %u + %u = %u\n", (unsigned)i, p1, p2, p3);
  sl_setp(guard, n);
}
sl_enddef

#define N 45
number fibonums[N];

sl_def(t_main, void)
{
  // first, compute the numbers.
  fibonums[0] = fibonums[1] = 1;
  sl_create(fid,,2,N,,,, 
	    fibo_compute, 
	    sl_sharg(number, prev, 1), 
	    sl_sharg(number, prev2, 1),
	    sl_glarg(number*, fibo, fibonums));
  sl_sync(fid);

  // then, print them.
  sl_create(,,2,N,,,,
	    fibo_print, 
	    sl_sharg(number, guard, 0), sl_glarg(number*, t, fibonums));
  sl_sync();
  
}
sl_enddef
