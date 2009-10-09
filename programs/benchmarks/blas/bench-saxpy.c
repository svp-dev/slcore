//
// bench-saxpy.c: this file is part of the SL toolchain.
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

#include <svp/testoutput.h>
#include <svp/perf.h>
#include <svp/fibre.h>
#include <svp/assert.h>
#include <cmalloc.h>

#include "benchmark.h"

struct bdata {
  long n;
  float a;
  float *sx;
  float *sy;
  double *sy_orig;
};

sl_def(initialize, void,
       sl_glparm(struct benchmark_state*, st))
{
  int i;
  struct bdata *bdata = (struct bdata*) malloc(sizeof(struct bdata));
  svp_assert(bdata != NULL);

  /* benchmark input:
     1 x ulong
     1 x double
     1 x double array (X)
     1 x double array (Y) */
  svp_assert(fibre_tag(0) == 0);
  svp_assert(fibre_rank(0) == 0);
  bdata->n = *(long*)fibre_data(0);

  svp_assert(fibre_tag(1) == 2);
  svp_assert(fibre_rank(1) == 0);
  bdata->a = *(double*)fibre_data(1);

  svp_assert(fibre_tag(2) == 2);
  svp_assert(fibre_rank(2) == 1);
  bdata->sx = (float*)malloc(bdata->n * sizeof(float));
  for (i = 0; i < bdata->n; ++i) bdata->sx[i] = ((double*)fibre_data(2))[i];

  svp_assert(fibre_tag(3) == 2);
  svp_assert(fibre_rank(3) == 1);
  svp_assert(fibre_shape(3)[0] >= bdata->n);
  bdata->sy_orig = (double*)fibre_data(3);

  bdata->sy = (float*)malloc(bdata->n * sizeof(float));
  svp_assert(bdata->sy != NULL);

  sl_getp(st)->data = (void*) bdata;
}
sl_enddef

sl_def(prepare, void,
       sl_glparm(struct benchmark_state*, st))
{
  struct bdata *bdata = (struct bdata*)sl_getp(st)->data;
  long i;
  for (i = 0; i < bdata->n; ++i)
    bdata->sy[i] = bdata->sy_orig[i];
}
sl_enddef

sl_def(output, void,
       sl_glparm(struct benchmark_state*, st))
{
  struct bdata *bdata = (struct bdata*)sl_getp(st)->data;
  long i;
  for (i = 0; i < bdata->n; ++i) {
    output_float(bdata->sy[i], 1, 4);
    output_char('\n', 1);
  }
}
sl_enddef

sl_def(teardown, void,
       sl_glparm(struct benchmark_state*, st))
{
  struct bdata *bdata = (struct bdata*)sl_getp(st)->data;
  free(bdata->sy);
  free(bdata->sx);
  free(bdata);
}
sl_enddef

#include "saxpy.c"

sl_def(work, void,
       sl_glparm(struct benchmark_state*, st))
{
  struct bdata *bdata = (struct bdata*)sl_getp(st)->data;
  sl_proccall(saxpy,
	      sl_glarg(long, n, bdata->n),
	      sl_glfarg(float, a, bdata->a),
	      sl_glarg(float*, sx, bdata->sx),
	      sl_glarg(long, incx, 1),
	      sl_glarg(float*, sy, bdata->sy),
	      sl_glarg(long, incy, 1));
}
sl_enddef

sl_def(t_main, void)
{
  struct benchmark b = {
    "BLAS: SAXPY",
    "kena",
    "Compute Y[i] = A * X[i] + Y[i] with single precision",
    &initialize, &prepare, &work, &output, &teardown
  };
  sl_proccall(run_benchmark, sl_glarg(struct benchmark*, b, &b));
}
sl_enddef
