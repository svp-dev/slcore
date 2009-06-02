//
// fft_test.sl: this file is part of the slc project.
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

m4_include(svp/iomacros.slh)
m4_include(svp/assert.slh)
m4_include(svp/perf.slh)
m4_include(slr.slh)

m4_include(fft.slh)



static cpx_t X[1 << 16];
static cpx_t Y[1 << 16];
static cpx_t Z[1 << 16];

sl_def(fft_init, void)
{
  sl_index(i);

  X[i].re = Y[i].re = i + 1;
  X[i].im = Y[i].im = 0.;
}
sl_enddef

sl_def(copy_y_z, void)
{
  sl_index(i);

  Z[i].re = Y[i].re;
  Z[i].im = Y[i].im;
}
sl_enddef

sl_def(print_int, void, sl_shparm(int, guard), sl_glparm(cpx_t*restrict, array))
{
  sl_index(i);
  int g = sl_getp(guard);
  cpx_t *restrict A = sl_getp(array);

  long long C = 10000;
  int d = i;
  printf("%d  |  %d %d\n", d,
	 (int64_t)(C*A[i].re), (int64_t)(C*A[i].im));

  sl_setp(guard, g);
}
sl_enddef

sl_def(print_fl, void, sl_shparm(int, guard), sl_glparm(cpx_t*restrict, array))
{
  sl_index(i);
  int g = sl_getp(guard);
  cpx_t *restrict A = sl_getp(array);

  int d = i;
  printf("%d  |  %g %g\n", d,
	 A[i].re, A[i].im);

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

sl_def(t_main, void)
{
  svp_assert(slr_len(M) > 0);
  unsigned M = slr_get(M)[0];
  svp_assert(M <= 16);
  svp_assert(M <= (sizeof(sc_table)/sizeof(sc_table[0])));

  unsigned N = 1 << M;

  sl_create(,,,N,,,, fft_init);
  sl_sync();

  int64_t p1, p2;
  int br = (slr_len(BR) > 0);

  p1 = get_cycles();
  sl_create(,,,,,,, FFT, sl_glarg(cpx_t*restrict, gX, Y), sl_glarg(unsigned long, gM, M), sl_glarg(long, gBR, br));
  sl_sync();
  p2 = get_cycles();
  if (slr_len(Pc) > 0)
    printf("%d %d\n", p1, (p2 - p1));
  else {
    sl_create(,,,N,,,, copy_y_z);
    sl_sync();

    sl_create(,,,,,,, FFT_Inv, sl_glarg(cpx_t*restrict, gZ, Z), sl_glarg(unsigned long, gM2, sl_geta(gM)));
    sl_sync();

    if (slr_len(Pf) > 0) {
      puts("   |  X\n");
      sl_create(,,,N,,,, print_fl, sl_sharg(int, g1, 0), sl_glarg(cpx_t*restrict, a1, X));
      sl_sync();
      puts("   |  Y = FFT(X)\n");
      sl_create(,,,N,,,, print_fl, sl_sharg(int, g2, 0), sl_glarg(cpx_t*restrict, a2, Y));
      sl_sync();
      puts("   |  Z = InvFFT(Y)\n");
      sl_create(,,,N,,,, print_fl, sl_sharg(int, g3, 0), sl_glarg(cpx_t*restrict, a3, Z));
      sl_sync();
    }

    if (slr_len(Pi) > 0) {
      puts("   |  int(X*10000)\n");
      sl_create(,,,N,,,, print_int, sl_sharg(int, g1, 0), sl_glarg(cpx_t*restrict, a1, X));
      sl_sync();
      puts("   |  int(Y*10000) Y=FFT(X)\n");
      sl_create(,,,N,,,, print_int, sl_sharg(int, g2, 0), sl_glarg(cpx_t*restrict, a2, Y));
      sl_sync();
      puts("   |  int(Z*10000) Z=InvFFT(Y)\n");
      sl_create(,,,N,,,, print_int, sl_sharg(int, g3, 0), sl_glarg(cpx_t*restrict, a3, Z));
      sl_sync();
    }
  }
}
sl_enddef
