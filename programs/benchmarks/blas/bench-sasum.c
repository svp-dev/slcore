//
// bench-sasum.c: this file is part of the SL toolchain.
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
  float *sx;
  float result;
};

sl_def(initialize, void,
       sl_glparm(struct benchmark_state*, st))
{
  int i;
  struct bdata *bdata = (struct bdata*) malloc(sizeof(struct bdata));
  svp_assert(bdata != NULL);

  /* benchmark input:
     1 x ulong
     1 x double array (X) */
  svp_assert(fibre_tag(0) == 0);
  svp_assert(fibre_rank(0) == 0);
  bdata->n = *(long*)fibre_data(0);

  svp_assert(fibre_tag(1) == 2);
  svp_assert(fibre_rank(1) == 1);
  bdata->sx = (float*)malloc(fibre_shape(1)[0] * sizeof(float));
  svp_assert(bdata->sx != NULL);
  for (i = 0; i < fibre_shape(1)[0]; ++i) bdata->sx[i] = ((double*)fibre_data(1))[i];

  sl_getp(st)->data = (void*) bdata;
}
sl_enddef

sl_def(output, void,
       sl_glparm(struct benchmark_state*, st))
{
  struct bdata *bdata = (struct bdata*)sl_getp(st)->data;
  output_float(bdata->result, 1, 4);
  output_char('\n', 1);
}
sl_enddef

sl_def(teardown, void,
       sl_glparm(struct benchmark_state*, st))
{
  struct bdata *bdata = (struct bdata*)sl_getp(st)->data;
  free(bdata->sx);
  free(bdata);
}
sl_enddef

#include "sasum.c"

sl_def(work, void,
       sl_glparm(struct benchmark_state*, st))
{
  struct bdata *bdata = (struct bdata*)sl_getp(st)->data;
  sl_create(,,,,,,, sasum,
	    sl_shfarg(float, result, 0.),
	    sl_glarg(long, n, bdata->n),
	    sl_glarg(float*, sx, bdata->sx),
	    sl_glarg(long, incx, 1));
  sl_sync();
  bdata->result = sl_geta(result);
}
sl_enddef

sl_def(t_main, void)
{
  struct benchmark b = {
    "BLAS: SASUM",
    "kena",
    "Compute D = D + ABS(X[i]) with single precision",
    &initialize, 0, &work, &output, &teardown
  };
  sl_proccall(run_benchmark, sl_glarg(struct benchmark*, b, &b));
}
sl_enddef
