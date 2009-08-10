#include <svp/slr.h>
#include <svp/perf.h>
#include <svp/iomacros.h>

slr_decl(slr_var(size_t, L, "number of iterations"),
	 slr_var(size_t, N, "problem size"));

#define MAX_N 10*1024*1024


sl_def(ll4_inner, void,
       sl_glparm(double*restrict, X),
       sl_glparm(double*restrict, Y),
       sl_glparm(unsigned long, i),
       sl_shfparm(double, temp))
{
  sl_index(j);
  sl_setp(temp, sl_getp(temp) + sl_getp(X)[sl_getp(i) + j - 6] + sl_getp(Y)[j * 5 + 4]);
}
sl_enddef



sl_def(ll4_outer, void, 
       sl_glparm(double*restrict, X),
       sl_glparm(double*restrict, Y),
       sl_glparm(unsigned long, N5))
{
  sl_index(i);

  double*restrict X = sl_getp(X);
  double*restrict Y = sl_getp(Y);
  double t1, t2;
  sl_create(,,0,sl_getp(N5),,,, ll4_inner,
	    sl_glarg(double*restrict, gX, X),
	    sl_glarg(double*restrict, gY, Y),
	    sl_glarg(unsigned long, gi, i),
	    sl_shfarg(double, temp, 0));
  t1 = X[i - 1];
  t2 = Y[4];
  sl_sync();
  X[i-1] = t2 * (t1 - sl_geta(temp));
}
sl_enddef


double X[MAX_N], Y[MAX_N];

// SLT_RUN:  L=1 N=10000
// SLT_RUN:  L=3 N=1000
// XIGNORE: *:D

sl_def(t_main, void)
{
  size_t L = slr_get(L)[0];
  size_t N = slr_get(N)[0];

  uint64_t counts[L];
  size_t i;

  unsigned long m = (N - 7) / 2;
  unsigned long n5 = N / 5;

  for (i = 0; i < L; ++i) {
    uint64_t c1 = get_cycles();
    
    sl_create(,,6,N,m,,, ll4_outer,
	      sl_glarg(double*restrict, gX, X),
	      sl_glarg(double*restrict, gY, Y),
	      sl_glarg(unsigned long, gN5, n5));
    sl_sync();

    counts[i] = get_cycles() - c1;
  }
  for (i = 0; i < L; ++i) 
    printf("%u\n", counts[i]);
}
sl_enddef
