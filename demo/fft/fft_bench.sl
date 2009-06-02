//
// fft_bench.sl: this file is part of the slc project.
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

m4_include(slr.slh)
m4_include(fft.slh)

static cpx_t X[1 << 16];

slr_decl(slr_var(unsigned, M, "problem size"));

sl_def(t_main, void)
{
  unsigned M = slr_get(M)[0];
  unsigned N = 1 << M;

  sl_create(,PLACE_LOCAL,1,M+1,1,,, FFT_1,
	    sl_glarg(cpx_t*restrict, gX, X),
	    sl_glarg(unsigned long, gN, N/2),
	    sl_sharg(long, token, 0),
	    sl_glarg(const void*, gt, sc_table));
  sl_sync();
}
sl_enddef
