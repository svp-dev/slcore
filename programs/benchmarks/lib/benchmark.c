//
// benchmark.c: this file is part of the SL toolchain.
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

#include "benchmark.h"
#include <svp/perf.h>
#include <svp/slr.h>
#include <svp/abort.h>
#include <svp/testoutput.h>
#include <svp/iomacros.h>

#ifdef puts
#undef puts
#endif
#define puts(S) output_string((S), 1)
#ifdef putc
#undef putc
#endif
#define putc(C) output_char((C), 1)

slr_decl(slr_var(unsigned, ncores, "number of cores (default 1)"),
	 slr_var(unsigned, L, "number of outer iterations (default 3)"),
	 slr_var(int, format, "format for benchmark results (default=0=fibre, 1=raw)"),
	 slr_var(int, results, "output computation results (default 0=no)"),
	 slr_var(int, sep_dump, "output initial place configuration (default 0=no)"));

sl_def(run_benchmark, void, sl_glparm(struct benchmark*, b))
{
  /* configuration from environment */
  unsigned ncores = 1;
  unsigned L = 3;
  int results = 0;
  int format = 0;
  int sep_dump = 0;

  if (slr_len(ncores)) ncores = slr_get(ncores)[0];
  if (slr_len(L)) L = slr_get(L)[0];
  if (slr_len(results)) results = slr_get(results)[0];
  if (slr_len(format)) format = slr_get(format)[0];
  if (slr_len(sep_dump)) sep_dump = slr_get(sep_dump)[0];

  /* extract benchmark callbacks */
  struct benchmark *b = sl_getp(b);
  sl_decl((*initialize), void, sl_glparm(struct benchmark_state*, state));
  sl_decl((*prepare), void, sl_glparm(struct benchmark_state*, state));
  sl_decl((*work), void, sl_glparm(struct benchmark_state*, state));
  sl_decl((*output), void, sl_glparm(struct benchmark_state*, state));
  sl_decl((*teardown), void, sl_glparm(struct benchmark_state*, state));
  initialize = b->initialize;
  prepare = b->prepare;
  work = b->work;
  output = b->output;
  teardown = b->teardown;

  /* some introduction */
  if (!b->title) b->title = "(unnamed)";
  if (!b->author) b->author = "(anonymous)";
  printf("####\n#### %s\n####\n## By: %s\n", b->title, b->author);
  if (b->description)
    printf("## %s\n", b->description);
  putc('\n');

  unsigned nsamples = 5+2*L;
  struct ct_t { counter_t vals[MTPERF_NCOUNTERS]; } ct[nsamples][2];
  struct ct_t ct_zero = { { 0 } };
  int is = 0;

  /* prepare benchmark state and place */

  struct benchmark_state bs;
  sl_place_t pid;

  bs.data = 0;

  printf("# 1. initial place allocation (ncores=%u)...", ncores);
#if SVP_HAS_SEP
  {
    mtperf_sample(ct[is][0].vals);
    sl_create(,root_sep->sep_place|1,,,,,, root_sep->sep_alloc,
	      sl_glarg(struct SEP*, _0, root_sep),
	      sl_glarg(unsigned long, _1, SAL_EXACT|ncores),
	      sl_sharg(struct placeinfo*, p, 0));
    sl_sync();
    mtperf_sample(ct[is++][1].vals);
    if (!sl_geta(p)) {
      output_string("Place allocation failed!\n", 2);
      svp_abort();
    }
    bs.place = sl_geta(p);
    pid = bs.place->pid;

    if (sep_dump) {
      puts("# SEP status dump after initial allocation:\n");
      sl_create(,root_sep->sep_place|1,,,,,, root_sep->sep_dump_info,
		sl_glarg(struct SEP*, _2, root_sep));
      sl_sync();
    }
  }
  puts("ok\n");
#else
  puts("no SEP, using default place\n");
  pid = PLACE_DEFAULT;
  ct[is][0] = ct[is][1] = ct_zero; ++is;
#endif

  puts("# 2. initialize...");
  if (initialize) {
    mtperf_sample(ct[is][0].vals);
    sl_proccall(initialize, sl_glarg(struct benchmark_state*, _0, &bs));
    mtperf_sample(ct[is++][1].vals);
    puts("ok\n");
  } else {
    puts("(nothing to do)\n");
    ct[is][0] = ct[is][1] = ct_zero; ++is;
  }

  int i;
  for (i = 0; i < L; ++i) {
    printf("# 3.%u prepare...", i+1);
    if (prepare) {
      mtperf_sample(ct[is][0].vals);
      sl_proccall(prepare, sl_glarg(struct benchmark_state*, _0, &bs));
      mtperf_sample(ct[is++][1].vals);
      puts("ok\n");
    } else {
      puts("(nothing to do)\n");
      ct[is][0] = ct[is][1] = ct_zero; ++is;
    }

    printf("# 3.%u work...", i+1);
    mtperf_sample(ct[is][0].vals);
    sl_create(, pid,,,,,, work, sl_glarg(struct benchmark_state*, _0, &bs));
    sl_sync();
    mtperf_sample(ct[is++][1].vals);
    puts("ok\n");
  }

  puts("# 4. results...");
  if (results && output) {
    putc('\n');
    mtperf_sample(ct[is][0].vals);
    sl_proccall(output, sl_glarg(struct benchmark_state*, _0, &bs));
    mtperf_sample(ct[is++][1].vals);
    putc('\n');
  } else {
    puts("(nothing to do)\n");
    ct[is][0] = ct[is][1] = ct_zero; ++is;
  }

  puts("# 5. teardown...");
  if (teardown) {
    mtperf_sample(ct[is][0].vals);
    sl_proccall(teardown, sl_glarg(struct benchmark_state*, _0, &bs));
    mtperf_sample(ct[is++][1].vals);
    puts("ok\n");
  } else {
    puts("(nothing to do)\n");
    ct[is][0] = ct[is][1] = ct_zero; ++is;
  }
  puts("# done.\n\n");

  output_string("## measurements:\n", 2);
  for (i = 0; i < is; ++i) 
    mtperf_report_diffs(ct[i][0].vals, ct[i][1].vals, 
			(format ? (REPORT_CSV|CSV_SEP(' ')) : REPORT_FIBRE) | REPORT_STREAM(2));

}
sl_enddef
