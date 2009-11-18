//
// perf.c: this file is part of the SL toolchain.
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

#include <svp/perf.h>
#include <svp/testoutput.h>
#include <cmalloc.h>

// the ordering in the following
// table should match the counter
// indices in perf.h.
static const char* cnames[] = {
  "clocks",
#ifdef __mt_freestanding__
  "exec_insns",
  "issued_flops",
#endif
};

void mtperf_report_diffs(const counter_t* before, const counter_t* after, int flags)
{
  int i;

  int stream = (flags >> 24) & 0xff;
  if (!stream) stream = 1;
  int format = (flags >> 8) & 0xff;
  int fflags = flags & 0xff;
  switch (format) {
  case 0:
    {
      // output CSV
      int print_headers = fflags & 1;
      int spec_sep = fflags & 2;
      char sep = (flags >> 16) & 0xff;
      if (print_headers) {
	// print column headers
	for (i = 0; i < MTPERF_NCOUNTERS; ++i) {
	  if (i != 0) output_char(spec_sep ? sep : ',', stream);
	  output_string(cnames[i], stream);
	}
	output_char('\n', stream);
      }
      for (i = 0; i < MTPERF_NCOUNTERS; ++i) {
	if (i != 0) output_char(spec_sep ? sep : ',', stream);
	output_int(after[i]-before[i], stream);
      }
      output_char((fflags & REPORT_NOLF) ? ' ' : '\n', stream);
    }
    break;
  case 1:
    // output raw
    for (i = 0; i < MTPERF_NCOUNTERS; ++i) {
      output_int(after[i]-before[i], stream);
      output_char((fflags & REPORT_NOLF) ? ' ' : '\n', stream);
    }
    break;
  case 2:
    // output Fibre
    output_char('[', stream);
    output_char('0', stream);
    output_char(',', stream);
    output_int(MTPERF_NCOUNTERS-1, stream);
    output_char(':', stream);
    for (i = 0; i < MTPERF_NCOUNTERS; ++i) {
      output_char(' ', stream);
      output_int(after[i]-before[i], stream);
    }
    output_char(']', stream);
    output_char((fflags & REPORT_NOLF) ? ' ' : '\n', stream);
    break;
  }
}

void mtperf_report_intervals(const struct s_interval* ivs,
			     size_t n,
			     int flags)
{
  size_t i;

  int format = (flags >> 8) & 0xff;
  int stream = (flags >> 24) & 0xff;
  if (!stream) stream = 1;

  if (format != 0)
    output_string("### begin intervals\n", stream);

  for (i = 0; i < n; ++i)
    {
      mtperf_report_diffs(ivs[i].before, ivs[i].after, flags | REPORT_NOLF);
      output_string(" #", stream);
      if (ivs[i].num >= 0) {
	output_char(' ', stream);
	output_uint(ivs[i].num, stream);
      }
      if (ivs[i].tag) {
	output_char(' ', stream);
	output_string(ivs[i].tag, stream);
      }
      if (!(flags & REPORT_NOLF))
	output_char('\n', stream);
      flags &= ~CSV_INCLUDE_HEADER;
    }

  if (format != 0)
    output_string("### end intervals\n", stream);
}

struct s_interval* mtperf_alloc_intervals(size_t n) 
{ 
  return (struct s_interval*)fast_calloc(n, sizeof(struct s_interval)); 
}

void mtperf_free_intervals(struct s_interval* ivs)
{
  free(ivs);
}

#undef mtperf_start_interval
void mtperf_start_interval(struct s_interval* ivs, size_t p,
			   int numarg,
			   const char *tag)
{
  mtperf_start_interval_(ivs, p, numarg, tag);
}

#undef mtperf_empty_interval
void mtperf_empty_interval(struct s_interval* ivs, size_t p,
			   int numarg,
			   const char *tag)
{
  mtperf_empty_interval_(ivs, p, numarg, tag);
}

#undef mtperf_finish_interval
void mtperf_finish_interval(struct s_interval* ivs, size_t p)
{
  mtperf_finish_interval_(ivs, p);
}


