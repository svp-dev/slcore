//
// fft_extra.c: this file is part of the SL toolchain.
//
// Copyright (C) 2009 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

sl_def(FFT_Swap, void,
       sl_glparm(cpx_t*restrict, X),
       sl_glparm(unsigned long, i),
       sl_glparm(unsigned long, j))
{
  if (sl_getp(i) < sl_getp(j)) {
    cpx_t *restrict Xi = sl_getp(X) + sl_getp(i);
    cpx_t *restrict Xj = sl_getp(X) + sl_getp(j);
    cpx_t T1 = *Xi, T2 = *Xj;
    *Xi = T2; *Xj = T1;
  }
}
sl_enddef

sl_def(FFT_Reverse, void,
       sl_glparm(cpx_t*restrict, X),
       sl_glparm(unsigned long, N),
       sl_shparm(unsigned long, j))
{
  sl_index(i);
  unsigned long t = sl_getp(j);
  sl_create(,,,,,,, FFT_Swap,
	    sl_glarg(cpx_t*restrict, gX, sl_getp(X)),
	    sl_glarg(unsigned long, gi, i),
	    sl_glarg(unsigned long, gt, t));

  // NOTE: so much code between create and sync
  // below has 'unspecified behavior' in SL.

  unsigned long k = sl_getp(N) ;
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
       sl_glparm(cpx_t*restrict, X),
       sl_glparm(unsigned long, M),
       sl_glparm(long, do_bitreversal))
{
  unsigned long N = 1 << sl_getp(M);

  if (sl_getp(do_bitreversal)) {
    sl_create(,,,N-1,,,, FFT_Reverse,
	      sl_glarg(cpx_t*restrict, gX, sl_getp(X)),
	      sl_glarg(unsigned long, gN, N/2),
	      sl_sharg(unsigned long, j, 0));
    sl_sync();
  }

  sl_create(,PLACE_LOCAL,1,sl_getp(M)+1,1,,, FFT_1,
	    sl_glarg(cpx_t*restrict, gX, sl_getp(X)),
	    sl_glarg(unsigned long, gN, N/2),
	    sl_sharg(long, token, 0),
	    sl_glarg(const void*, gt, sc_table));
  sl_sync();
}
sl_enddef

sl_def(Conjugate, void, sl_glparm(cpx_t*restrict, X))
{
  sl_index(i);
  cpx_t *X = sl_getp(X);

  X[i].im = -X[i].im;
}
sl_enddef

sl_def(Scale, void,
       sl_glparm(cpx_t*restrict, X),
       sl_glparm(unsigned long, N))
{
  sl_index(i);
  cpx_t *X = sl_getp(X);

  X[i].re = X[i].re / sl_getp(N);
  X[i].im = -X[i].im / sl_getp(N);
}
sl_enddef

sl_def(FFT_Inv, void,
       sl_glparm(cpx_t*restrict, X),
       sl_glparm(unsigned long, M))
{
  unsigned long N = 1 << sl_getp(M);

  sl_create(,,,N,,,, Conjugate,
	    sl_glarg(cpx_t*restrict, gX, sl_getp(X)));
  sl_sync();


  sl_create(,,,,,,, FFT,
	    sl_glarg(cpx_t*restrict, gX2, sl_geta(gX)),
	    sl_glarg(unsigned long, gM, sl_getp(M)),
	    sl_glarg(long, gDBR, 1));
  sl_sync();

  sl_create(,,,N,,,, Scale,
	    sl_glarg(cpx_t*restrict, gX3, sl_geta(gX2)),
	    sl_glarg(unsigned long, gN, N));
  sl_sync();

}
sl_enddef
