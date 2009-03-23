m4_include(svp/iomacros.slh)

m4_define(FT, double)

typedef FT cpx_t[2];

m4_include(fft_table.slh)

m4_define(RE, 0)
m4_define(IM, 1)

/* for (i = j; i < N; i += LE) fft3(...) */
sl_def(fft3, void,
       sl_glparm(cpx_t*, X),
       sl_glparm(unsigned, LE2),
       sl_glfparm(FT, Ure),
       sl_glfparm(FT, Uim))
{
  sl_index(i);

  unsigned ip = i + sl_getp(LE2);

  cpx_t *X = sl_getp(X);
  FT re = X[ip][RE];

  /* complex T = X[i] + X[ip] */
  FT Tre = X[i][RE] + re;
  FT Tim = X[i][IM] + X[ip][IM];

  /* X[ip] = (X[i] - X[ip]) * U; */
  X[ip][RE] = (X[i][RE] - re) * sl_getp(Ure) - (X[i][IM] - X[ip][IM]) * sl_getp(Uim);
  X[ip][IM] = (X[i][IM] - X[ip][IM]) * sl_getp(Ure) + (X[i][RE] - re) * sl_getp(Uim);

  /* X[i] = T */
  X[i][RE] = Tre; 
  X[i][IM] = Tim;
}
sl_enddef

/* for (j = 0; j < LE2; j++) fft2(...) */
sl_def(fft2, void,
       sl_shfparm(FT, Ure), 
       sl_shfparm(FT, Uim),
       sl_glparm(cpx_t*, S),
       sl_glparm(cpx_t*, X),
       sl_glparm(unsigned, N),
       sl_glparm(unsigned, LE),
       sl_glparm(unsigned, LE2))
{
  sl_index(j);

  FT re = sl_getp(Ure);
  FT im = sl_getp(Uim);
  /* for (i = j; i < N; i += LE) ... */
  sl_create(,,j, sl_getp(N), sl_getp(LE),,,
	    fft3,
	    sl_glarg(cpx_t*, X, sl_getp(X)),
	    sl_glarg(unsigned, LE2, sl_getp(LE2)),
	    sl_glfarg(FT, gUre, re),
	    sl_glfarg(FT, gUim, im));
  sl_sync();

  /* U = U * S; */
  cpx_t *S = sl_getp(S);
  sl_setp(Ure, re * (*S)[RE] - im * (*S)[IM]);
  sl_setp(Uim, im * (*S)[RE] + re * (*S)[IM]);
}
sl_enddef

/* for (k = M; k > 0; k--) fft1(...) */
sl_def(fft1, void,
       sl_glparm(cpx_t*, X),
       sl_glparm(unsigned, N))
{
  sl_index(k);

  unsigned LE = 1U << k;
  unsigned LE2 = LE / 2U;

  /* for (j = 0; j < LE2; j++) ... */
  sl_create(,, 0, LE2,,,, 
	    fft2,
	    sl_shfarg(FT, Ure, 1.0),
	    sl_shfarg(FT, Uim, 0.0),
	    sl_glarg(cpx_t*, S, &sc_table[k]),
	    sl_glarg(cpx_t*, X, sl_getp(X)),
	    sl_glarg(unsigned, N, sl_getp(N)),
	    sl_glarg(unsigned, gLE, LE),
	    sl_glarg(unsigned, gLE2, LE2));

  sl_sync();

}
sl_enddef

/* swap X[i] and X[j] if i < j */
sl_def(fft_post_swap, void,
       sl_glparm(unsigned, i),
       sl_glparm(unsigned, j),
       sl_glparm(cpx_t*, X))
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

/* for (i = 0, j = 0; i < N - 1; i++) fft_post(...) */
sl_def(fft_post, void,
       sl_shparm(unsigned, j),
       sl_glparm(cpx_t*, X),
       sl_glparm(unsigned, N2))
{
  sl_index(i);

  /* swap X[i] and X[j] */
  sl_create(,,,,,,, fft_post_swap, 
	    sl_glarg(unsigned, gi, i),
	    sl_glarg(unsigned, gj, sl_getp(j)),
	    sl_glarg(cpx_t*, gX, sl_getp(X)));
  sl_sync();

  unsigned k = sl_getp(N2);
  unsigned j = sl_getp(j);
  while (k - 1 < j)
    {
      j = j - k;
      k = k / 2;
    }
  sl_setp(j, j + k);
}
sl_enddef

sl_def(fft, void, 
       sl_glparm(cpx_t*, X),
       sl_glparm(unsigned, M))
{
  unsigned N = 1 << sl_getp(M);
  {
    /* for (k = M; k > 0; k--) ... */
    sl_create(,,sl_getp(M),0,-1,,,
	      fft1, 
	      sl_glarg(cpx_t*, gX, sl_getp(X)), 
	      sl_glarg(unsigned, gN, N));
    sl_sync();
  }
  {
    /* for (i = 0, j = 0; i < N - 1; i++) ... */
    sl_create(,,0,N-1,,,,
	      fft_post, 
	      sl_sharg(unsigned, j, 0), 
	      sl_glarg(cpx_t*, gX, sl_getp(X)),
	      sl_glarg(unsigned, N2, N/2));
    sl_sync();
  }
}
sl_enddef

sl_def(inv_fft1, void, sl_glparm(cpx_t*, X))
{
  sl_index(i);
  cpx_t* X = sl_getp(X);
  X[i][IM] = -X[i][IM];
}
sl_enddef

sl_def(inv_fft2, void, 
       sl_glparm(cpx_t*, X), 
       sl_glparm(unsigned, N))
{
  sl_index(i);
  cpx_t* X = sl_getp(X);
  X[i][RE] = X[i][RE] / sl_getp(N);
  X[i][IM] = -X[i][IM] / sl_getp(N);
}
sl_enddef


sl_def(inv_fft, void, 
       sl_glparm(cpx_t*, X),
       sl_glparm(unsigned, M))
{
  unsigned N = 1 << sl_getp(M);
  {
    sl_create(,,0,N,,,, inv_fft1,
	      sl_glarg(cpx_t*, gX, sl_getp(X)));
    sl_sync();
  }
  {
    sl_create(,,,,,,, fft, 
	      sl_glarg(cpx_t*, gX, sl_getp(X)),
	      sl_glarg(unsigned, gM, sl_getp(M)));
    sl_sync();
  }
  {
    sl_create(,,0,N,,,, inv_fft2,
	      sl_glarg(cpx_t*, gX, sl_getp(X)),
	      sl_glarg(unsigned, gN, N));
    sl_sync();
  }
}
sl_enddef

m4_define(M_INIT, 4)
m4_define(N_INIT, (1 << M_INIT))

cpx_t X[N_INIT];
cpx_t Y[N_INIT];
cpx_t Z[N_INIT];

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

sl_def(print, void, sl_shparm(int, guard))
{
  sl_index(i);
  int g = sl_getp(guard);

  printf("X = %f %f\tY = %f %f\tZ = %f %f\n",
	 X[i][RE], X[i][IM],
	 Y[i][RE], Y[i][IM],
	 Z[i][RE], Z[i][IM]);

  sl_setp(guard, g);
}
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,,N_INIT,,,, fft_init);
  sl_sync();

  sl_create(,,,,,,, fft, sl_glarg(cpx_t*, gX, Y), sl_glarg(unsigned, gM, M_INIT));
  sl_sync(); 

  sl_create(,,,N_INIT,,,, copy_y_z);
  sl_sync();

  sl_create(,,,,,,, inv_fft, sl_glarg(cpx_t*, gZ, Z), sl_glarg(unsigned, gM2, M_INIT));
  sl_sync();

  sl_create(,,,N_INIT,,,, print, sl_sharg(int, guard, 0));
  sl_sync();
}
sl_enddef

