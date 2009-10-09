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

#include <ctime.h>

#define get_cycles() m4_warning("get_cycles() is deprecated. use clock() and CLOCKS_PER_SEC instead.") clock()

typedef long counter_t;

#ifdef __mt_freestanding__
#define MTPERF_CLOCKS 0
#define MTPERF_EXECUTED_INSNS 1
#define MTPERF_ISSUED_FP_INSNS 2
#define MTPERF_NCOUNTERS 3
typedef struct { counter_t ct[MTPERF_NCOUNTERS]; } __counters_t;
#define mtperf_sample(Array) do { \
  *(__counters_t*restrict)(void*)(Array) = *(volatile __counters_t*restrict)(void*)-256; \
  } while(0)

#define mtperf_sample1(Counter) (((volatile __counters_t*restrict)(void*)-256)->ct[Counter])

#else

#define MTPERF_NCOUNTERS 1
#define MTPERF_CLOCKS 0
#define mtperf_sample(Array) do { (Array)[0] = (counter_t)clock(); } while(0)
#define mtperf_sample1(Counter) (((Counter) == 0) ? clock() : -1)

#endif

extern void mtperf_report_diffs(const counter_t *before, const counter_t* after, int flags);

#define CSV_SEP(C) ((((int)(C))<<16)|2)
#define CSV_INCLUDE_HEADER 1
#define REPORT_CSV 0
#define REPORT_FIBRE (2<<8)
#define REPORT_RAW (1<<8)
#define REPORT_STREAM(N) ((N)<<24)

#endif // ! SLC_SVP_PERF_H
