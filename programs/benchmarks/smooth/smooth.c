//
// smooth.c: this file is part of the SL program suite.
//
// Copyright (C) 2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//
#include <svp/fibre.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <svp/slr.h>
#include <svp/perf.h>
#include <svp/testoutput.h>
#include <svp/sep.h>
#include "benchmark.h"

static
void print2d_square(double * a, size_t N)
{
    output_char('[',1);
    output_uint(N,1);
    output_char(':',1);
    for (size_t i = 0; i < N; ++i) 
    {
        output_char('[',1);
        output_uint(N,1);
        output_char(':',1);        
        for (size_t j = 0; j < N; ++j)
        {
            output_char(' ', 1);
            output_float(a[i*N+j], 1, 6);
        }
        output_char(']',1);
        output_char('\n',1);
    }
    output_char(']',1);
    output_char('\n',1);
    

}

#define A(i) a[i] // *(double*restrict)(((char*)a) + (i))
#define B(i) b[i] // *(double*restrict)(((char*)b) + (i))
#define one 1

sl_def(convo_inner, (sl__naked(native), sl__static),
       sl_glparm(size_t, w), 
       sl_glparm(double*restrict, a),
       sl_glparm(double*restrict, b),
       sl_glfparm(double, c8))
{
    sl_index(i);
    size_t w = sl_getp(w);
    double *restrict a = sl_getp(a);
    double *restrict b = sl_getp(b);


    B(i) = (A(i-w-one) + A(i-w  ) + A(i-w+one) +
            A(i  -one)            + A(i  +one) +
            A(i+w-one) + A(i+w  ) + A(i+w+one)) * sl_getp(c8);
}
sl_enddef

sl_def(convo_outer, sl__static,
       sl_glparm(size_t, w),
       sl_glparm(double*restrict, a),
       sl_glparm(double*restrict, b),
       sl_glfparm(double, c8))
{
    sl_index(i);

    size_t m = i + sl_getp(w) - one;

    sl_create(,PLACE_LOCAL, i+one, m, one, ,sl__naked(native), convo_inner, 
              sl_glarg(size_t,,sl_getp(w)), 
              sl_glarg(double*restrict,,sl_getp(a)), 
              sl_glarg(double*restrict,,sl_getp(b)), 
              sl_glfarg(double,c8));
    sl_seta(c8, sl_getp(c8));
#ifdef __mt_freestanding__
    asm("":::"memory");
#endif
    sl_getp(b)[i/one] = sl_getp(a)[i/one];
    sl_getp(b)[m/one] = sl_getp(a)[m/one];
    sl_sync();
}
sl_enddef

sl_def(copy, sl__static,
       sl_glparm(double*restrict, a),
       sl_glparm(double*restrict, b))
{
    sl_index(i);
    sl_getp(b)[i] = sl_getp(a)[i];
}
sl_enddef


struct bdata {
    size_t N; // problem size
    double *restrict a;
    double *restrict b;
};

sl_def(initialize, void, sl_glparm(struct benchmark_state*, st))
{
    struct bdata *bdata = (struct bdata*)malloc(sizeof(struct bdata));

    size_t i;

#ifndef READ_FIBRE
#ifndef FIXED_SIZE
#define FIXED_SIZE 100
#endif
    bdata->N = FIXED_SIZE;
    size_t data_size = bdata->N * bdata->N;
    bdata->a = (double*)malloc(data_size*sizeof(double));
    assert(bdata->a != 0);
    for (i = 0; i < data_size; ++i)
        bdata->a[i] = i;
#else
    assert(fibre_tag(0) == 2);
    assert(fibre_rank(0) == 2);
    bdata->N = fibre_shape(0)[0];
    size_t N2 = fibre_shape(0)[1];

    assert(bdata->N == N2);

    size_t data_size = N * N2;

    bdata->a = (double*)fibre_data(0);
#endif

    bdata->b = (double*)malloc(data_size*sizeof(double));
    assert(bdata->b != NULL);
    for (i = 0; i < data_size; ++i)
        bdata->b[i] = bdata->a[i];

    sl_getp(st)->data = bdata;
}
sl_enddef


sl_def(work, void, sl_glparm(struct benchmark_state*, st))
{
  struct work_lapses * wl = sl_getp(st)->wl;
  struct bdata *bdata = (struct bdata*)sl_getp(st)->data;

  size_t N = bdata->N, data_size = N * N;
  double *restrict a = bdata->a;
  double *restrict b = bdata->b;

#ifdef ITERS
  size_t i;

  for (i = 0; i < ITERS; ++i)
  {
      start_interval(wl, "convolution");
#endif
      sl_create(,,N, (data_size-N), N, 2,, convo_outer, 
                sl_glarg(size_t,, N), 
                sl_glarg(double*restrict,,a), 
                sl_glarg(double*restrict,,b), 
                sl_glfarg(double, , 1.0/8.0));
      sl_create(,,0, N, 1, 4,, copy,
                sl_glarg(double*restrict,,a),
                sl_glarg(double*restrict,,b));
      sl_create(,,(data_size-N), data_size, 1, 4,, copy,
                sl_glarg(double*restrict,,a),
                sl_glarg(double*restrict,,b));
      sl_sync();
      sl_sync();
      sl_sync();
#ifdef ITERS
      finish_interval(wl);
      double *t = a;
      a = b;
      b = t;
  }
  bdata->a = a;
  bdata->b = b;
#endif
}
sl_enddef

sl_def(output, void,        sl_glparm(struct benchmark_state*, st))
{
    struct bdata *bdata = (struct bdata*)sl_getp(st)->data;
    print2d_square(bdata->a, bdata->N);
 
}   
sl_enddef

sl_def(t_main, void)
{
  struct benchmark b = {
    "Simple image 'smoothing'",
    "kena",
    "Average the floating point data across a 2D grid",
    &initialize, 0, &work, &output, 0
  };
  sl_proccall(run_benchmark, sl_glarg(struct benchmark*, , &b));

}
sl_enddef
