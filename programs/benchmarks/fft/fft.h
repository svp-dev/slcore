//
// fft.h: this file is part of the SL program suite.
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

#ifndef SL_BENCHMARKS_FFT_FFT_H
# define SL_BENCHMARKS_FFT_FFT_H

#include "benchmark.h"

#define FT double

typedef struct { FT re; FT im; } cpx_t;

/* low-level FFT (for benchmarks) */

sl_decl(FFT_1, void,
       sl_glparm(cpx_t*restrict, X),
       sl_glparm(unsigned long, N),
       sl_shparm(long, token),
       sl_glparm(const void*, t));

sl_decl(FFT_Inv, void,
	sl_glparm(cpx_t*restrict, X),
	sl_glparm(unsigned long, M),
	sl_glparm(struct work_lapses*, wl));

sl_decl(FFT, void,
       sl_glparm(cpx_t*restrict, X),
       sl_glparm(unsigned long, M),
       sl_glparm(struct work_lapses*, wl),
       sl_glparm(const char*, wname));

#define STRINGIFY_(N) # N
#define STRINGIFY(N) STRINGIFY_(N)
#define MAKENAME_(N, SZ) fft_table ## N ## _ ## SZ ## _data.h
#define MAKENAME(N, SZ) MAKENAME_(N, SZ)


#endif // ! SL_BENCHMARKS_FFT_FFT_H
