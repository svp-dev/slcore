//
// blas-test-saxpy.sl: this file is part of the slc project.
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

// compile with slc blas-test-saxpy.sl blas/saxpy.sl

#include <svp/perf.h>
#include <svp/iomacros.h>
#include <svp/slr.h>

slr_decl(slr_var(long, N, "problem size"));

#define HZ 1000000000

float x[1000000] = { 3.5 };
float y[1000000] = { 3.5 };

sl_decl(saxpy, void,
       sl_glparm(long, n),
       sl_glfparm(float, a),
       sl_glparm(float*, sx),
       sl_glparm(long, incx),
       sl_glparm(float*, sy),
	sl_glparm(long, incy));

// SLT_RUN: N=1000
// XIGNORE: *:D

sl_def(t_main, void)
{
  int64_t c1, c2;
  long N = slr_get(N)[0];
  c1 = get_cycles();
  sl_proccall(saxpy,
	      sl_glarg(long, n, N),
	      sl_glfarg(float, a, 2.0),
	      sl_glarg(float*, sx, x),
	      sl_glarg(long, incx, 1),
	      sl_glarg(float*, sy, y),
	      sl_glarg(long, incy, 1));
  c2 = get_cycles();

  float time = (float)(c2 - c1) / (float)HZ;
  float saxpy = N;
  float cycle = (float)(c2 - c1);
  printf("%d\t%d\t%d\t%f\t%f\t%f\t%f\n",
	 N, 1, c2-c1, saxpy, time, saxpy/time, saxpy/cycle);
}
sl_enddef

#include "saxpy.c"
