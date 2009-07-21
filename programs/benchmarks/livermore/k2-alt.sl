m4_include(slr.slh)
m4_include(svp/perf.slh)
m4_include(svp/iomacros.slh)

slr_decl(slr_var(size_t, L, "number of iterations"),
	 slr_var(size_t, N, "problem size"));

m4_define([[MAX_N]], 10*1024*1024)


sl_def(ll2_inner, void,
       sl_glparm(double*restrict, X),
       sl_glparm(double*restrict, V),
       sl_glparm(unsigned long, ipnt),
       sl_glparm(unsigned long, ipntp))
{
  sl_index(i);
  unsigned long k = sl_getp(ipnt) + i;
  double*restrict X = sl_getp(X);
  double*restrict V = sl_getp(V);
  X[sl_getp(ipntp) + i / 2 + 1] = X[k] - V[k] * X[k-1] - V[k+1] * X[k+1];
}
sl_enddef



sl_def(ll2_outer, void, 
       sl_glparm(double*restrict, X),
       sl_glparm(double*restrict, V),
       sl_shparm(unsigned long, ipntp))
{
  sl_index(m);

  unsigned long ii = 1U << m;
  unsigned long ipnt = sl_getp(ipntp);
  unsigned long ipntp = ipnt + ii;
  sl_create(,,1,ii,2,,, ll2_inner,
	    sl_glarg(double*restrict, gX, sl_getp(X)),
	    sl_glarg(double*restrict, gV, sl_getp(V)),
	    sl_glarg(unsigned long, gipnt, ipnt),
	    sl_glarg(unsigned long, gipntp, ipntp));
  sl_sync();
  sl_setp(ipntp, ipntp);

}
sl_enddef


double X[MAX_N], V[MAX_N];

// SLT_RUN:  L=1 N=8
// SLT_RUN:  L=3 N=10
// XIGNORE: *:D

sl_def(t_main, void)
{
  size_t L = slr_get(L)[0];
  size_t N = slr_get(N)[0];

  uint64_t counts[L];
  size_t i;

  for (i = 0; i < L; ++i) {
    uint64_t c1 = get_cycles();
    
    sl_create(,,N,0,-1,,, ll2_outer,
	      sl_glarg(double*restrict, gX, X),
	      sl_glarg(double*restrict, gV, V),
	      sl_sharg(unsigned long, ipntp, 0));
    sl_sync();

    counts[i] = get_cycles() - c1;
  }
  for (i = 0; i < L; ++i) 
    printf("%u\n", counts[i]);
}
sl_enddef
