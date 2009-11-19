
#include <svp/testoutput.h>
#include <svp/fibre.h>
#include <svp/perf.h>
#include <svp/assert.h>
#include <cmalloc.h>

#include "benchmark.h"

#define TABLE_SIZE 16
#include "fft.h"

struct bdata {
  const cpx_t * restrict x_orig;
  cpx_t * restrict y_fft;
  cpx_t * restrict z_inv;
  size_t M;
  size_t N;
};

sl_def(initialize, void,
       sl_glparm(struct benchmark_state*, st))
{
  int i;
  struct bdata *bdata = (struct bdata*) malloc(sizeof(struct bdata));
  svp_assert(bdata != 0);

  svp_assert(2*sizeof(double) == sizeof(cpx_t));

  svp_assert(fibre_tag(0) == 0);
  svp_assert(fibre_rank(0) == 0);
  bdata->M = *(unsigned long*)fibre_data(0);
  svp_assert(bdata->M <= TABLE_SIZE);
  bdata->N = 1 << bdata->M;

  svp_assert(fibre_tag(1) == 2);
  svp_assert(fibre_rank(1) == 2);
  svp_assert(fibre_shape(1)[0] >= bdata->N);
  svp_assert(fibre_shape(1)[1] == 2);
  bdata->x_orig = (const cpx_t*)fibre_data(1);

  bdata->y_fft = (cpx_t*)malloc(sizeof(cpx_t) * bdata->N);
  svp_assert(bdata->y_fft != 0);
  
  bdata->z_inv = (cpx_t*)malloc(sizeof(cpx_t) * bdata->N);
  svp_assert(bdata->z_inv != 0);

  sl_getp(st)->data = bdata;
}
sl_enddef

sl_def(prepare, void,
       sl_glparm(struct benchmark_state*, st))
{
  struct bdata *bdata = (struct bdata*)sl_getp(st)->data;  
  int i;
  for (i = 0; i < bdata->N; ++i)
    bdata->y_fft[i] = bdata->x_orig[i];
}
sl_enddef

sl_def(work, void,
       sl_glparm(struct benchmark_state*, st))
{
  struct bdata *bdata = (struct bdata*)sl_getp(st)->data;  
  struct work_lapses *wl = sl_getp(st)->wl;
  int i;
  sl_create(,,,,,,, FFT,
	    sl_glarg(cpx_t*restrict, , bdata->y_fft),
	    sl_glarg(unsigned long, , bdata->M),
	    sl_glarg(struct work_lapses*, , wl),
	    sl_glarg(const char*, , "work1"));
  sl_sync();

  start_interval(wl, "copy");
  for (i = 0; i < bdata->N; ++i)
    bdata->z_inv[i] = bdata->y_fft[i];
  finish_interval(wl);

  sl_create(,,,,,,, FFT_Inv,
	    sl_glarg(cpx_t*restrict, , bdata->z_inv),
	    sl_glarg(unsigned long, , bdata->M),
	    sl_glarg(struct work_lapses*, , wl));
  sl_sync();

}
sl_enddef


sl_def(print_fl, void, 
       sl_shparm(long, guard), 
       sl_glparm(cpx_t*restrict, array))
{
  sl_index(i);
  cpx_t A = sl_getp(array)[i];

  long g = sl_getp(guard);
  output_int(i, 1);
  output_char('\t', 1);
  output_float(A.re, 1, 6);
  output_char('\t', 1);
  output_float(A.im, 1, 6);
  output_char('\n', 1);
  sl_setp(guard, g);
}
sl_enddef

sl_def(output, void,
       sl_glparm(struct benchmark_state*, st))
{
  struct bdata *bdata = (struct bdata*)sl_getp(st)->data;  
  output_string("# Forward FFT:\n", 1);
  sl_create(,,, bdata->N,,,, print_fl,
	    sl_sharg(long, , 0), sl_glarg(cpx_t*restrict, , bdata->y_fft));
  sl_sync();
  output_string("# Reverse FFT:\n", 1);
  sl_create(,,, bdata->N,,,, print_fl,
	    sl_sharg(long, , 0), sl_glarg(cpx_t*restrict, , bdata->z_inv));
  sl_sync();
}
sl_enddef

sl_def(teardown, void,
       sl_glparm(struct benchmark_state*, st))
{
  struct bdata *bdata = (struct bdata*)sl_getp(st)->data;  
  free(bdata->y_fft);
  free(bdata->z_inv);
  free(bdata);
}
sl_enddef

sl_def(t_main, void)
{
  struct benchmark b = {
    "FFT 1D",
    "kena",
    "Perform FFT and inverse FFT over a 1D vector " EXTRA_COMMENT,
    &initialize, &prepare, &work, &output, &teardown
  };
  sl_proccall(run_benchmark, sl_glarg(struct benchmark*, b, &b));

}
sl_enddef
