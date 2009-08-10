//
// fft_impl1.c: this file is part of the slc project.
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
// $Id$
//

#define HEADERNAME MAKENAME(1, TABLE_SIZE)
#define HEADER STRINGIFY(HEADERNAME)

#include HEADER

const void* sc_table_ptr = sc_table;

sl_def(FFT_2, void,
       sl_glparm(cpx_t*restrict, X),
       sl_glparm(unsigned long, k),
       sl_glparm(const void*, t))
{
  sl_index(i);
  unsigned long   km1 = sl_getp(k) - 1;
  unsigned long   LE2 = 1 << km1;
  unsigned long   w   = i & (LE2 - 1); // ((i >> km1) * LE2);
  unsigned long   j   = 2 * (i - w) + w;
  unsigned long   ip  = j + LE2;
  typeof(sc_table)* restrict lsc_table = (typeof(sc_table)*)(const void*)sl_getp(t);

  cpx_t U = (*lsc_table)[km1][i];

  cpx_t *restrict X = sl_getp(X);
  cpx_t T = { U.re * X[ip].re - U.im * X[ip].im,
	      U.im * X[ip].re + U.re * X[ip].im };

  cpx_t Xj = X[j];
  X[ip].re = Xj.re - T.re;
  X[ip].im = Xj.im - T.im;
  X[j] .re = Xj.re + T.re;
  X[j] .im = Xj.im + T.im;
}
sl_enddef

sl_def(FFT_1, void,
       sl_glparm(cpx_t*restrict, X),
       sl_glparm(unsigned long, N2),
       sl_shparm(long, token),
       sl_glparm(const void*, t))
{
  sl_index(k);

  int t = sl_getp(token);
  sl_create(,,0,sl_getp(N2),,,, FFT_2,
	    sl_glarg(cpx_t*restrict, gX, sl_getp(X)),
	    sl_glarg(unsigned long, gk, k),
	    sl_glarg(const void*, gt, sl_getp(t)));
  sl_sync();
  sl_setp(token, t);
}
sl_enddef

