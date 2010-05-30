//
// sep.c: this file is part of the SL program suite.
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

#include <cstdio.h>
#include <svp/sep.h>
#include <svp/perf.h>
#include <svp/slr.h>

#include <svp/testoutput.h>

// SLT_RUN: L=1 nmax=4
// XIGNORE: *:D mta_s*:R

slr_decl(slr_var(unsigned, L, "number of rounds"),
	 slr_var(unsigned, nmax, "max nr policies used"));

sl_def(t_main, void)
{
#if SVP_HAS_SEP
  static struct reqs {
    const char *desc;
    unsigned long policy;
    struct placeinfo *result;
  } reqs[] = {
#define STRFY(Val) #Val
#define MAKEREQ(Val) { STRFY(Val), Val, 0 }
    MAKEREQ(SAL_DONTCARE),
    MAKEREQ(SAL_DONTCARE),
    MAKEREQ(SAL_DONTCARE),
    MAKEREQ(SAL_MIN|4),
    MAKEREQ(SAL_MAX|4),
    MAKEREQ(SAL_MIN|4),
    MAKEREQ(SAL_MAX|4),
    MAKEREQ(SAL_EXACT|16),
    MAKEREQ(SAL_DONTCARE),
    MAKEREQ(SAL_MIN|4),
    MAKEREQ(SAL_MAX|4),
    MAKEREQ(SAL_MIN|4),
    MAKEREQ(SAL_MAX|4),
    MAKEREQ(SAL_EXACT|16),
    MAKEREQ(SAL_DONTCARE),
    MAKEREQ(SAL_MIN|4),
    MAKEREQ(SAL_MAX|4),
    MAKEREQ(SAL_MIN|4),
    MAKEREQ(SAL_MAX|4),
    MAKEREQ(SAL_EXACT|16),
    MAKEREQ(SAL_DONTCARE),
    MAKEREQ(SAL_MIN|4),
    MAKEREQ(SAL_MAX|4),
    MAKEREQ(SAL_MIN|4),
    MAKEREQ(SAL_MAX|4),
    MAKEREQ(SAL_EXACT|16),
    MAKEREQ(SAL_DONTCARE),
    MAKEREQ(SAL_DONTCARE),
    MAKEREQ(SAL_DONTCARE),
  };
  size_t N = sizeof(reqs) / sizeof(reqs[0]);
  counter_t ct[N+1][MTPERF_NCOUNTERS];

  unsigned n;
  unsigned L = 4;
  if (slr_len(L)) L = slr_get(L)[0];
  if (slr_len(nmax) && slr_get(nmax)[0] < N) N = slr_get(nmax)[0];

  for (n = 0; n < L; ++n) {

    int i;
    for (i = 0; i < N; ++i) {
      mtperf_sample(ct[i]);
      sl_create(,root_sep->sep_place|1,,,,,, *root_sep->sep_alloc,
		sl_glarg(struct SEP*, , root_sep),
		sl_glarg(unsigned long, , reqs[i].policy),
		sl_sharg(struct placeinfo*, p, 0));
      sl_sync();
      reqs[i].result = sl_geta(p);
    }
    mtperf_sample(ct[i]);

    puts("Allocation results:\n");
    for (i = 0; i < N; ++i) {
      const char *success = reqs[i].result ? "yes" : "no";
      size_t ncores = reqs[i].result ? reqs[i].result->ncores : 0;
      printf("Round: %u\tPolicy: %s\tSucceded: %s (%u)\tPerformance: ",
	     n, reqs[i].desc, success, ncores);
      mtperf_report_diffs(ct[i], ct[i+1], REPORT_CSV|CSV_SEP(' '));
    }

    puts("Status dump (after alloc):\n");
    sl_create(,root_sep->sep_place|1,,,,,, *root_sep->sep_dump_info,
	      sl_glarg(struct SEP*, , root_sep));
    sl_sync();


    // dealloc

    for (i = 0; i < N; ++i) {
      mtperf_sample(ct[i]);
      if (reqs[i].result) {
	sl_create(,root_sep->sep_place|1,,,,,, *root_sep->sep_free,
		  sl_glarg(struct SEP*, , root_sep),
		  sl_glarg(struct placeinfo*, , reqs[i].result));
	sl_sync();
      }
    }
    mtperf_sample(ct[i]);

    puts("Deallocation results:\n");
    for (i = 0; i < N; ++i) {
      printf("Round: %u\tPolicy: %s", n, reqs[i].desc);
      if (reqs[i].result) {
	puts("\tPerformance: ");
	mtperf_report_diffs(ct[i], ct[i+1], REPORT_CSV|CSV_SEP(' '));
      } else puts("\t(allocation failed, nothing to do)\n");
    }

    puts("Status dump (after free):\n");
    sl_create(,root_sep->sep_place|1,,,,,, *root_sep->sep_dump_info,
	      sl_glarg(struct SEP*, , root_sep));
    sl_sync();


  }


#else
  puts("SEP protocol not supported.\n");
#endif
}
sl_enddef
