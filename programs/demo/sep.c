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

#include <stdio.h>
#include <svp/sep.h>
#include <svp/perf.h>
#include <svp/slr.h>

#include <svp/testoutput.h>

// SLT_RUN: L=1 nmax=15
// XIGNORE: *:D

slr_decl(slr_var(size_t, L, "number of rounds"),
	 slr_var(size_t, nmax, "max nr policies used"),
         slr_var(int, dump, "dump status between reqs"));

static
int lrand(void)
{
    static unsigned long long next = 0;
    next = next * 0x5deece66dLL + 11;
    return (int)((next >> 16) & 0x7fffffff);
}

#define STRFY(Val) #Val
#define MAKEREQ(Val) { STRFY(Val), Val }

#if SVP_HAS_SEP
struct req {
    const char *desc;
    unsigned long policy;
};

static
struct req sizes[] = {
    MAKEREQ(SAL_DONTCARE),
    MAKEREQ(SAL_DONTCARE),
    MAKEREQ(SAL_DONTCARE),
    MAKEREQ(SAL_MIN|4),
    MAKEREQ(SAL_MIN|4),
    MAKEREQ(SAL_MAX|4),
    MAKEREQ(SAL_MAX|4),
    MAKEREQ(SAL_EXACT|16)
};

struct req modes[] = {
    MAKEREQ(0),
    MAKEREQ(SAL_EXCLUSIVE),
    MAKEREQ(SAL_SHARED),
    MAKEREQ(SAL_SHARED|SAL_EXCLUSIVE)
};

struct results {
    const char *modedesc;
    const char *sizedesc;
    struct placeinfo *result;
};

#define MAXTRIES 50
static
struct results res[MAXTRIES];

#endif

sl_def(t_main, void)
{
#if SVP_HAS_SEP

  struct SEP* restrict sep = root_sep;
  size_t N = sizeof(res) / sizeof(res[0]);
  const size_t nmodes = sizeof(modes) / sizeof(modes[0]);
  const size_t nsizes = sizeof(sizes) / sizeof(sizes[0]);
  int dump = 0;

  size_t n;
  size_t L = 4;
  if (slr_len(L)) L = slr_get(L)[0];
  if (slr_len(nmax) && slr_get(nmax)[0] < N) N = slr_get(nmax)[0];
  if (slr_len(dump)) dump = 1;

  struct s_interval ct[2*N*L];
  int cti = 0;

  for (n = 0; n < L; ++n) {
    size_t i;
    
    if (dump) {
        printf("Status dump (before alloc %u):\n", n);
        sl_create(,sep->sep_place,,,,,, *sep->sep_dump_info,
                  sl_glarg(struct SEP*, , sep));
        sl_sync();
    }
    
    for (i = 0; i < N; ++i) {
        int r = lrand();
        r = (r & 0xff) ^ ((r >> 8) & 0xff) ^ ((r >> 16) & 0xff);
        struct req *m = &modes[r % nmodes];
        r = lrand();
        r = (r & 0xff) ^ ((r >> 8) & 0xff) ^ ((r >> 16) & 0xff);
        struct req *s = &sizes[r % nsizes];

        res[i].modedesc = m->desc;
        res[i].sizedesc = s->desc;
        unsigned long policy = m->policy | s->policy;

        mtperf_start_interval(ct, cti, i, "alloc");
        sl_create(,sep->sep_place,,,,, sl__exclusive, *sep->sep_alloc,
                  sl_glarg(struct SEP*, , sep),
                  sl_glarg(unsigned long, , policy),
                  sl_sharg(struct placeinfo*, p, 0));
        sl_sync();
        mtperf_finish_interval(ct, cti++);

        res[i].result = sl_geta(p);

        const char *success = res[i].result ? "yes" : "no";
        size_t ncores = res[i].result ? res[i].result->ncores : 0;
        printf("\nRound: %u %u\tPolicy: %s|%s\tSucceeded: %s (%u)\n",
               n, i, res[i].modedesc, res[i].sizedesc, success, ncores);

        if (dump && res[i].result) {
            printf("Status dump (after alloc %u %u):\n", n, i);
            sl_create(,sep->sep_place,,,,,, *sep->sep_dump_info,
                      sl_glarg(struct SEP*, , sep));
            sl_sync();
        }
    }

    // dealloc

    for (i = 0; i < N; ++i) {
        if (res[i].result) {
            mtperf_start_interval(ct, cti, i, "dealloc");
            sl_create(,sep->sep_place,,,,, sl__exclusive, *sep->sep_free,
                      sl_glarg(struct SEP*, , sep),
                      sl_glarg(struct placeinfo*, , res[i].result));
            sl_sync();
            mtperf_finish_interval(ct, cti++);

            if (dump) {
                printf("\nStatus dump (after free %u %u):\n", n, i);
                sl_create(,sep->sep_place,,,,,, *sep->sep_dump_info,
                          sl_glarg(struct SEP*, , sep));
                sl_sync();
            }
        }
        else
            mtperf_empty_interval(ct, cti++, i, "dealloc");
    }


  }

  printf("\nPerformance report (%d):\n", cti);
  mtperf_report_intervals(ct, cti, REPORT_CSV|CSV_INCLUDE_HEADER|CSV_SEP('\t'));

#else
  printf("SEP protocol not supported.\n");
#endif
}
sl_enddef
