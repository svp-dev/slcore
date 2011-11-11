//
// perf_wrappers.c: this file is part of the SL toolchain.
//
// Copyright (C) 2011 The SL project.
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

#undef mtperf_sample1
counter_t mtperf_sample1(int thecounter)
{
    return __inline_mtperf_sample1(thecounter);
}

#undef mtperf_sample
void mtperf_sample(counter_t* where)
{
    __inline_mtperf_sample(where);
}

#undef mtperf_start_interval
void mtperf_start_interval(struct s_interval* ivs, size_t p,
			   int numarg,
			   const char *tag)
{
    __inline_mtperf_start_interval(ivs, p, numarg, tag);
}

#undef mtperf_empty_interval
void mtperf_empty_interval(struct s_interval* ivs, size_t p,
			   int numarg,
			   const char *tag)
{
    __inline_mtperf_empty_interval(ivs, p, numarg, tag);
}

#undef mtperf_finish_interval
void mtperf_finish_interval(struct s_interval* ivs, size_t p)
{
    __inline_mtperf_finish_interval(ivs, p);
}


#undef mtperf_alloc_intervals
struct s_interval *
mtperf_alloc_intervals(size_t n)
{
    return (struct s_interval*)__inline_mtperf_alloc_intervals(n);
}

#undef mtperf_free_intervals
void mtperf_free_intervals(struct s_interval *ivs)
{
    __inline_mtperf_free_intervals(ivs);
}
