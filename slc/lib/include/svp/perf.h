//
// perf.h: this file is part of the SL toolchain.
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

#ifndef SLC_SVP_PERF_H
# define SLC_SVP_PERF_H

#ifndef shutup_ctime_h
#define shutup_ctime_h
#endif
#include <ctime.h>
#undef shutup_ctime_h

#define get_cycles() m4_warning("get_cycles() is deprecated. use clock() and CLOCKS_PER_SEC instead.") clock()

typedef long counter_t;

#ifdef __mt_freestanding__
#define MTPERF_CLOCKS 0
#define MTPERF_EXECUTED_INSNS 1
#define MTPERF_ISSUED_FP_INSNS 2
#define MTPERF_NCOUNTERS 3
typedef struct { counter_t ct[MTPERF_NCOUNTERS]; } __counters_t;

extern const char *mtpef_counter_names[];

#define mtperf_sample(Array) do {					\
    __asm__ __volatile__("# sample begins");				\
    *(__counters_t*restrict)(void*)(Array) = *(volatile __counters_t*restrict)(void*)8; \
    __asm__ __volatile__("# sample ends");				\
  } while(0)

#define mtperf_sample1(Counter) (((volatile __counters_t*restrict)(void*)8)->ct[Counter])

#else

#define MTPERF_NCOUNTERS 1
#define MTPERF_CLOCKS 0
#define mtperf_sample(Array) do { (Array)[0] = (counter_t)clock(); } while(0)
#define mtperf_sample1(Counter) (((Counter) == 0) ? clock() : -1)

#endif

extern void mtperf_report_diffs(const counter_t *before, const counter_t* after, int flags);

#define CSV_SEP(C) ((((int)(C))<<16)|2)
#define CSV_INCLUDE_HEADER 1
#define REPORT_NOLF 4
#define REPORT_CSV 0
#define REPORT_FIBRE (2<<8)
#define REPORT_RAW (1<<8)
#define REPORT_STREAM(N) ((N)<<24)
#define FIBRE_PAD(N) (((int)(N))<<16)

struct s_interval {
  counter_t before[MTPERF_NCOUNTERS];
  counter_t after[MTPERF_NCOUNTERS];
  const char *tag;
  int num;
};

void mtperf_empty_interval(struct s_interval* ivs, size_t p,
			   int numarg, const char *tag);
#define mtperf_empty_interval_(Ivs, P, Numarg, Tag) do {	  \
    struct s_interval __init = { { 0 }, { 0 }, (Tag), (Numarg) }; \
    (Ivs)[P] = __init;						  \
  } while(0)
#define mtperf_empty_interval mtperf_empty_interval_

void mtperf_start_interval(struct s_interval* ivs, size_t p,
			   int numarg,
			   const char *tag);
#define mtperf_start_interval_(Ivs, P, Numarg, Tag) do {		\
    struct s_interval* __ivs = (Ivs);					\
    size_t __p = (P);							\
    __ivs[__p].tag = (Tag);						\
    __ivs[__p].num = (Numarg);						\
    mtperf_sample(__ivs[__p].before);					\
  } while(0)
#define mtperf_start_interval mtperf_start_interval_

void mtperf_finish_interval(struct s_interval* ivs, size_t p);
#define mtperf_finish_interval_(Ivs, P) mtperf_sample((Ivs)[P].after)
#define mtperf_finish_interval mtperf_finish_interval_

void mtperf_report_intervals(const struct s_interval* ivs, size_t n, int flags);

// The following is simply an alias to calloc(n, sizeof(struct s_interval))
struct s_interval* mtperf_alloc_intervals(size_t n);
// The following is simply an alias to free(p)
void mtperf_free_intervals(struct s_interval* p);

#endif // ! SLC_SVP_PERF_H
