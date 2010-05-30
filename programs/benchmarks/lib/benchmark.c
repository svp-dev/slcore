//
// benchmark.c: this file is part of the SL program suite.
//
// Copyright (C) 2009,2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include <svp/perf.h>
#include <svp/slr.h>
#include <svp/testoutput.h>
#include <cstdlib.h>
#include <cstdio.h>
#include <cstdlib.h>
#include "benchmark.h"

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
  sl_decl_fptr(initialize, void, sl_glparm(struct benchmark_state*, state));
  sl_decl_fptr(prepare, void, sl_glparm(struct benchmark_state*, state));
  sl_decl_fptr(work, void, sl_glparm(struct benchmark_state*, state));
  sl_decl_fptr(output, void, sl_glparm(struct benchmark_state*, state));
  sl_decl_fptr(teardown, void, sl_glparm(struct benchmark_state*, state));
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

  /* prepare intervals and lapses */
  struct s_interval *intervals;
  intervals = (struct s_interval*)fast_calloc(4 + L + 10 * L, sizeof (struct s_interval));

  struct work_lapses wl = { 0, 0, intervals };

  /* prepare benchmark state and place */

  struct benchmark_state bs;
  bs.wl = &wl;
  bs.data = 0;
  sl_place_t pid;
  size_t p = 0;

  printf("# 1. initial place allocation (ncores=%u)...", ncores);
#if SVP_HAS_SEP
  {
    mtperf_start_interval(intervals, p, -1, "sep_alloc");
    sl_create(,root_sep->sep_place|1,,,,,, *root_sep->sep_alloc,
	      sl_glarg(struct SEP*, , root_sep),
	      sl_glarg(unsigned long, , SAL_EXACT|ncores),
	      sl_sharg(struct placeinfo*, p, 0));
    sl_sync();
    mtperf_finish_interval(intervals, p++);

    if (!sl_geta(p)) {
      output_string("Place allocation failed!\n", 2);
      abort();
    }
    bs.place = sl_geta(p);
    pid = bs.place->pid;

    if (sep_dump) {
      puts("done\n# SEP status dump after initial allocation:\n");
      sl_create(,root_sep->sep_place|1,,,,,, *root_sep->sep_dump_info,
		sl_glarg(struct SEP*, , root_sep));
      sl_sync();
    }
  }
  puts("ok\n");
#else
  puts("no SEP, using default place\n");
  pid = PLACE_DEFAULT;
  mtperf_empty_interval(intervals, p++, -1, "sep_alloc");
#endif

  puts("# 2. initialize...");
  if (initialize) {
    mtperf_start_interval(intervals, p, -1, "initialize");
    sl_proccall(*initialize, sl_glarg(struct benchmark_state*, , &bs));
    mtperf_finish_interval(intervals, p++);
    puts("ok\n");
  } else {
    puts("(nothing to do)\n");
    mtperf_empty_interval(intervals, p++, -1, "initialize");
  }

  int i;
  for (i = 0; i < L; ++i) {
    printf("# 3.%u prepare...", i+1);
    if (prepare) {
      mtperf_start_interval(intervals, p, i, "prepare");
      sl_proccall(*prepare, sl_glarg(struct benchmark_state*, , &bs));
      mtperf_finish_interval(intervals, p++);
      puts("ok\n");
    } else {
      puts("(nothing to do)\n");
      mtperf_empty_interval(intervals, p++, i, "prepare");
    }

    printf("# 3.%u work...", i+1);
    wl.current_interval = p+1;
    wl.current_iter = i;
    mtperf_start_interval(intervals, p, i, "work");
    sl_create(, pid,,,,,, *work, sl_glarg(struct benchmark_state*, , &bs));
    sl_sync();
    mtperf_finish_interval(intervals, p);
    p = wl.current_interval;
    puts("ok\n");
  }

  puts("# 4. results...");
  if (results && output) {
    putc('\n');
    mtperf_start_interval(intervals, p, -1, "output");
    sl_proccall(*output, sl_glarg(struct benchmark_state*, , &bs));
    mtperf_finish_interval(intervals, p++);
    putc('\n');
  } else {
    puts("(nothing to do)\n");
    mtperf_empty_interval(intervals, p++, -1, "output");
  }

  puts("# 5. teardown...");
  if (teardown) {
    mtperf_start_interval(intervals, p, -1, "teardown");
    sl_proccall(*teardown, sl_glarg(struct benchmark_state*, _0, &bs));
    mtperf_finish_interval(intervals, p++);
    puts("ok\n");
  } else {
    puts("(nothing to do)\n");
    mtperf_empty_interval(intervals, p++, -1, "teardown");
  }
  puts("# done.\n\n");

  output_string("### begin benchmark results\n", 2);
  long report_flags = (format ? (REPORT_CSV|CSV_SEP(' ')|CSV_INCLUDE_HEADER) \
		       : REPORT_FIBRE) | REPORT_STREAM(2);

  mtperf_report_intervals(intervals, p, report_flags);
  output_string("### end benchmark results\n", 2);
}
sl_enddef
