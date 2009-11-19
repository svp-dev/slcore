#ifndef BLASBENCH_H
#define BLASBENCH_H

#include <cmalloc.h>
#include <svp/testoutput.h>
#include <svp/assert.h>
#include <svp/fibre.h>
#include "benchmark.h"

#define BEGIN_VARS struct bdata {
#define DEF_COUNTER(Var)  long Var;
#define DEF_SCALAR(Var)   FLOAT Var;
#define DEF_ARRAY_IN(Var) \
  const FLOAT * restrict Var;
#define DEF_ARRAY_INOUT(Var) \
  const FLOAT * restrict Var ## _orig; \
  FLOAT * restrict Var;
#define END_VARS };

#define BEGIN_READ \
  sl_def(initialize, void, sl_glparm(struct benchmark_state*, st)) \
  { \
    int i, f = 0;							\
    struct bdata *bdata = (struct bdata*) malloc(sizeof(struct bdata)); \
    svp_assert(bdata != NULL);						

#define READ_COUNTER(Var)				\
  svp_assert(fibre_tag(f) == 0 || fibre_tag(f) == 1);	\
  svp_assert(fibre_rank(f) == 0);			\
  bdata->Var = *(long*)fibre_data(f); ++f;

#define READ_SCALAR(Var)			\
  svp_assert(fibre_tag(f) == 2);		\
  svp_assert(fibre_rank(f) == 0);		\
  bdata->Var = *(FLOAT*)fibre_data(f); ++f;

#define READ_ARRAY_IN(Var, N)				 \
  svp_assert(fibre_tag(f) == 2);			 \
  svp_assert(fibre_rank(f) == 1);			 \
  svp_assert(fibre_shape(f)[0] >= bdata->N);		 \
  FLOAT *Var = (FLOAT*)malloc(bdata->N * sizeof(FLOAT)); \
  svp_assert(Var != 0); \
  for (i = 0; i < bdata->N; ++i) Var[i] = ((double*)fibre_data(f))[i]; \
  ++f;								       \
  bdata->Var = Var;

#define READ_ARRAY_INOUT(Var, N)				       \
  svp_assert(fibre_tag(f) == 2);			 \
  svp_assert(fibre_rank(f) == 1);			 \
  svp_assert(fibre_shape(f)[0] >= bdata->N);		 \
  FLOAT *Var = (FLOAT*)malloc(bdata->N * sizeof(FLOAT));	       \
  svp_assert(Var != 0);						       \
  for (i = 0; i < bdata->N; ++i) Var[i] = ((double*)fibre_data(f))[i]; \
  ++f;								       \
  bdata->Var ## _orig = Var;					       \
  bdata->Var = (FLOAT*)malloc(bdata->N * sizeof(FLOAT));	       \
  svp_assert(bdata->Var != 0);

#define END_READ		     \
  sl_getp(st)->data = (void*) bdata; \
	     } sl_enddef

#define BEGIN_PREPARE \
  sl_def(prepare, void, sl_glparm(struct benchmark_state*, st)) { \
    struct bdata *bdata = (struct bdata*)sl_getp(st)->data; \
    long i; 

#define RESET_ARRAY_INOUT(Var, N)		\
  for (i = 0; i < bdata->N; ++i)		\
    bdata->Var[i] = bdata->Var ## _orig[i];

#define END_PREPARE } sl_enddef

#define BEGIN_WORK \
  sl_def(work, void, sl_glparm(struct benchmark_state*, st)) { \
    struct bdata *bdata = (struct bdata*)sl_getp(st)->data; 

#define USE_VAR(Var) bdata->Var

#define END_WORK } sl_enddef


#define BEGIN_OUTPUT \
  sl_def(output, void, sl_glparm(struct benchmark_state*, st)) { \
    struct bdata *bdata = (struct bdata*)sl_getp(st)->data; \
    long i; 

#define PRINT_ARRAY(Var, N)					\
  for (i = 0; i < bdata->N; ++i)	{			\
    output_float(bdata->Var[i], 1, 4);				\
    output_char('\n', 1);					\
  }

#define PRINT_SCALAR(Var) \
  output_float(bdata->Var, 1, 4); \
  output_char('\n', 1);

#define PRINT_COUNTER(Var) \
  output_int(bdata->Var, 1); \
  output_char('\n', 1);

#define END_OUTPUT } sl_enddef

#define BEGIN_TEARDOWN \
  sl_def(teardown, void, sl_glparm(struct benchmark_state*, st)) { \
    struct bdata *bdata = (struct bdata*)sl_getp(st)->data; 

#define FREE_ARRAY_INOUT(Var)			\
  free((void*)bdata->Var ## _orig);		\
  free((void*)bdata->Var);

#define FREE_ARRAY_IN(Var)			\
  free((void*)bdata->Var);

#define END_TEARDOWN \
  free(bdata);	     \
  } sl_enddef

#define BEGIN_DESC \
  sl_def(t_main, void) { 					\
    struct benchmark b = { 

#define BENCH_TITLE(Title) Title,
#define BENCH_AUTHOR(Author) Author,

#define _STRFY(Var) # Var
#define STRFY(Var) _STRFY(Var)
#define BENCH_DESC(Desc) Desc " (using type " STRFY(FLOAT) ")",

#define END_DESC							\
  &initialize, &prepare, &work, &output, &teardown };			\
    sl_proccall(run_benchmark, sl_glarg(struct benchmark*, b, &b)); \
    } \
  sl_enddef
#endif


