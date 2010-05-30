//
// benchmark.h: this file is part of the SL program suite.
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

#ifndef SL_BENCHMARKS_LIB_BENCHMARK_H
# define SL_BENCHMARKS_LIB_BENCHMARK_H

#include <svp/sep.h>
#include <svp/perf.h>

struct work_lapses {
    size_t current_interval;
    size_t current_iter;
    struct s_interval *intervals;
};

struct benchmark_state {
    void* data;
#if SVP_HAS_SEP
    struct placeinfo *place;
#endif
    struct work_lapses * wl;
};

struct benchmark {
    const char *title;
    const char *author;
    const char *description;
    sl_decl_fptr(initialize, void, sl_glparm(struct benchmark_state*, state));
    sl_decl_fptr(prepare, void, sl_glparm(struct benchmark_state*, state));
    sl_decl_fptr(work, void, sl_glparm(struct benchmark_state*, state));
    sl_decl_fptr(output, void, sl_glparm(struct benchmark_state*, state));
    sl_decl_fptr(teardown, void, sl_glparm(struct benchmark_state*, state));
};

sl_decl(run_benchmark, void, sl_glparm(struct benchmark*, b));

#define start_interval(wl, Tag) \
  mtperf_start_interval(wl->intervals, wl->current_interval, wl->current_iter, (Tag))
#define finish_interval(wl) \
  mtperf_finish_interval(wl->intervals, wl->current_interval++)

#define start_finish_empty_interval(wl, Tag) \
  mtperf_empty_interval(wl->intervals, wl->current_interval++, wl->current_iter, (Tag))


#endif // ! SL_BENCHMARKS_LIB_BENCHMARK_H
