//
// fft_bench.c: this file is part of the slc project.
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

#include <svp/assert.h>
#include <svp/fibre.h>

#define TABLE_SIZE 16
#include "fft.h"

static cpx_t X[1 << TABLE_SIZE];
extern const void* sc_table_ptr;

sl_def(t_main, void)
{
  svp_assert(fibre_tag(0) == 0);
  svp_assert(fibre_rank(0) == 0);
  svp_assert(fibre_tag(1) == 0);
  svp_assert(fibre_rank(1) == 0);

  unsigned L = *(unsigned long*)fibre_data(0);
  unsigned M = *(unsigned long*)fibre_data(1);
  svp_assert(M < TABLE_SIZE);

  unsigned N = 1 << M;
  unsigned i;

  for (i = 0; i < L; ++i) {  
    sl_create(,PLACE_LOCAL,1,M+1,1,,, FFT_1,
	      sl_glarg(cpx_t*restrict, gX, X),
	      sl_glarg(unsigned long, gN, N/2),
	      sl_sharg(long, token, 0),
	      sl_glarg(const void*, gt, sc_table_ptr));
    sl_sync();
  }
}
sl_enddef
