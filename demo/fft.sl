m4_include(svp/iomacros.slh)
m4_include(svp/assert.slh)
m4_include(svp/perf.slh)
m4_include(slr.slh)

m4_define(FT, double)

typedef FT cpx_t[2];

m4_include(fft_table.slh)

m4_define(RE, 0)
m4_define(IM, 1)

sl_def(FFT_2, void, 
       sl_glparm(cpx_t*, X), 
       sl_glparm(unsigned, k))
{
  sl_index(i);
  unsigned   km1 = sl_getp(k) - 1;
  unsigned   LE2 = 1 << km1;
  unsigned   w   = i & (LE2 - 1); // ((i >> km1) * LE2);
  unsigned   j   = 2 * (i - w) + w;
  unsigned   ip  = j + LE2;

  FT Ure, Uim, Tre, Tim;
  Ure = sc_table[km1][i][RE];
  Uim = sc_table[km1][i][IM];

  cpx_t *X = sl_getp(X);
  Tre = Ure * X[ip][RE] - Uim * X[ip][IM];
  Tim = Uim * X[ip][RE] + Ure * X[ip][IM];

  X[ip][RE] = X[j][RE] - Tre;
  X[ip][IM] = X[j][IM] - Tim;
  X[j] [RE] = X[j][RE] + Tre;
  X[j] [IM] = X[j][IM] + Tim;
}
sl_enddef

sl_def(FFT_1, void,
       sl_glparm(cpx_t*, X),
       sl_glparm(unsigned, N),
       sl_shparm(int, token))
{
  sl_index(k);

  int t = sl_getp(token);
  sl_create(,,0,sl_getp(N)/2,,,, FFT_2, 
	    sl_glarg(cpx_t*, gX, sl_getp(X)), 
	    sl_glarg(unsigned, gk, k));
  sl_sync();
  sl_setp(token, t);
}
sl_enddef

/* swap X[i] and X[j] if i < j */
sl_def(FFT_Swap, void,
       sl_glparm(cpx_t*, X),
       sl_glparm(unsigned, i),
       sl_glparm(unsigned, j))
{
  if (sl_getp(i) < sl_getp(j)) {
    cpx_t *Xi = sl_getp(X) + sl_getp(i);
    cpx_t *Xj = sl_getp(X) + sl_getp(j);
    FT t1 = (*Xi)[RE];
    FT t2 = (*Xi)[IM];
    FT t3 = (*Xj)[RE];
    FT t4 = (*Xj)[IM];
    (*Xi)[RE] = t3; 
    (*Xi)[IM] = t4; 
    (*Xj)[RE] = t1; 
    (*Xj)[IM] = t2;
  }
}
sl_enddef

sl_def(FFT_Reverse, void,
       sl_glparm(cpx_t*, X),
       sl_glparm(unsigned, N),
       sl_shparm(unsigned, j))
{
  sl_index(i);
  unsigned t = sl_getp(j);
  sl_create(,,,,,,, FFT_Swap, 
	    sl_glarg(cpx_t*, gX, sl_getp(X)),
	    sl_glarg(unsigned, gi, i),
	    sl_glarg(unsigned, gt, t));

  // NOTE: so much code between create and sync
  // below has 'unspecified behavior' in SL.

  unsigned k = sl_getp(N) / 2;
  while (k <= t) 
    {
      t = t - k;
      k = k / 2;
    }
  sl_sync(); 

  sl_setp(j, t + k);
}
sl_enddef

sl_def(FFT, void,
       sl_glparm(cpx_t*, X),
       sl_glparm(unsigned, M),
       sl_glparm(int, do_bitreversal))
{
  unsigned N = 1 << sl_getp(M);

  if (sl_getp(do_bitreversal)) {
    sl_create(,,,N-1,,,, FFT_Reverse,
	      sl_glarg(cpx_t*, gX, sl_getp(X)),
	      sl_glarg(unsigned, gN, N),
	      sl_sharg(unsigned, j, 0));
    sl_sync();
  }
  
  sl_create(,PLACE_LOCAL,1,sl_getp(M)+1,1,,, FFT_1,
	    sl_glarg(cpx_t*, gX, sl_getp(X)),
	    sl_glarg(unsigned, gN, N),
	    sl_sharg(int, token, 0));
  sl_sync(); 
}
sl_enddef

sl_def(Conjugate, void, sl_glparm(cpx_t*, X))
{
  sl_index(i);
  cpx_t *X = sl_getp(X);
  
  X[i][IM] = -X[i][IM];
}
sl_enddef

sl_def(Scale, void, 
       sl_glparm(cpx_t*, X),
       sl_glparm(unsigned, N))
{
  sl_index(i);
  cpx_t *X = sl_getp(X);
  
  X[i][RE] = X[i][RE] / sl_getp(N);
  X[i][IM] = -X[i][IM] / sl_getp(N);
}
sl_enddef

sl_def(FFT_Inv, void,
       sl_glparm(cpx_t*, X),
       sl_glparm(unsigned, M))
{
  unsigned N = 1 << sl_getp(M);
  
  sl_create(,,,N,,,, Conjugate, 
	    sl_glarg(cpx_t*, gX, sl_getp(X)));
  sl_sync();


  sl_create(,,,,,,, FFT, 
	    sl_glarg(cpx_t*, gX2, sl_geta(gX)), 
	    sl_glarg(unsigned, gM, sl_getp(M)),
	    sl_glarg(int, gDBR, 1));
  sl_sync();

  sl_create(,,,N,,,, Scale, 
	    sl_glarg(cpx_t*, gX3, sl_geta(gX2)),
	    sl_glarg(unsigned, gN, N));
  sl_sync();

}
sl_enddef


cpx_t *X;
cpx_t *Y;
cpx_t *Z;

sl_def(fft_init, void)
{
  sl_index(i);

  X[i][RE] = Y[i][RE] = i + 1;
  X[i][IM] = Y[i][IM] = 0.;
}
sl_enddef

sl_def(copy_y_z, void)
{
  sl_index(i);

  Z[i][RE] = Y[i][RE];
  Z[i][IM] = Y[i][IM];
}
sl_enddef

sl_def(print_int, void, sl_shparm(int, guard), sl_glparm(cpx_t*, array))
{
  sl_index(i);
  int g = sl_getp(guard);
  cpx_t *A = sl_getp(array);
  long long d = i;
  long long C = 10000;
  printf("%d  |  %d %d\n", d, 
	 (long long)(C*A[i][RE]), (long long)(C*A[i][IM]));

  sl_setp(guard, g);
}
sl_enddef

sl_def(print_fl, void, sl_shparm(int, guard), sl_glparm(cpx_t*, array))
{
  sl_index(i);
  int g = sl_getp(guard);
  cpx_t *A = sl_getp(array);
  long long d = i;
  printf("%d  |  %g %g\n", d,
	 (double)A[i][RE], (double)A[i][IM]);

  sl_setp(guard, g);
}
sl_enddef

slr_decl(
	 slr_var(unsigned, M, "problem size"),
	 slr_var(int, BR, "if nonempty: perform bit reversal in forward FFT"),
	 slr_var(int, Pc, "if nonempty: print number of cycles for FFT and stop"),
	 slr_var(int, Pi, "if nonempty: print values as integers after computation"),
	 slr_var(int, Pf, "if nonempty: print values as floats after computation")
	 );

// SLT_RUN: -dM=3 -dPi -dPf=1 -dPc -dBR=1
// SLT_RUN: -dM=4 -dPi -dPf=1 -dPc -dBR=1
// SLT_RUN: -dM=4 -dPi -dPf=1 -dPc -dBR=1 -Ws,-o -Ws,NumProcessors=3

// FIXME: as of 2004-04-05 the output of this
// program seems implementation dependent;
// so ignore the diff during testing:
// XIGNORE: *:D

sl_def(t_main, void)
{
  svp_assert(slr_len(M) > 0);
  unsigned M = slr_get(M)[0];
  svp_assert(slr_len(M) <= (sizeof(sc_table)/sizeof(sc_table[0])));

  unsigned N = 1 << M;

  cpx_t lX[N]; X = lX;
  cpx_t lY[N]; Y = lY;
  cpx_t lZ[N]; Z = lZ;

  sl_create(,,,N,,,, fft_init);
  sl_sync();

  int64_t p1, p2;
  int br = (slr_len(BR) > 0);

  p1 = get_cycles();
  sl_create(,,,,,,, FFT, sl_glarg(cpx_t*, gX, Y), sl_glarg(unsigned, gM, M), sl_glarg(int, gBR, br));
  sl_sync(); 
  p2 = get_cycles();
  if (slr_len(Pc) > 0)
    printf("%d\n", (p2 - p1));
  else {
    sl_create(,,,N,,,, copy_y_z);
    sl_sync();

    sl_create(,,,,,,, FFT_Inv, sl_glarg(cpx_t*, gZ, Z), sl_glarg(unsigned, gM2, sl_geta(gM)));
    sl_sync();
    
    if (slr_len(Pf) > 0) {
      puts("   |  X\n");
      sl_create(,,,N,,,, print_fl, sl_sharg(int, g1, 0), sl_glarg(cpx_t*, a1, X));
      sl_sync();
      puts("   |  Y = FFT(X)\n");
      sl_create(,,,N,,,, print_fl, sl_sharg(int, g2, 0), sl_glarg(cpx_t*, a2, Y));
      sl_sync();
      puts("   |  Z = InvFFT(Y)\n");
      sl_create(,,,N,,,, print_fl, sl_sharg(int, g3, 0), sl_glarg(cpx_t*, a3, Z));
      sl_sync();
    }
    
    if (slr_len(Pi) > 0) {
      puts("   |  int(X*10000)\n");
      sl_create(,,,N,,,, print_int, sl_sharg(int, g1, 0), sl_glarg(cpx_t*, a1, X));
      sl_sync();
      puts("   |  int(Y*10000) Y=FFT(X)\n");
      sl_create(,,,N,,,, print_int, sl_sharg(int, g2, 0), sl_glarg(cpx_t*, a2, Y));
      sl_sync();
      puts("   |  int(Z*10000) Z=InvFFT(Y)\n");
      sl_create(,,,N,,,, print_int, sl_sharg(int, g3, 0), sl_glarg(cpx_t*, a3, Z));
      sl_sync();
    }
  }
}
sl_enddef

