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
	  if (i != 0) output_char(spec_sep ? sep : ',', 1);
	  output_string(cnames[i], 1);
	}
	output_char('\n', 1);
      }
      for (i = 0; i < MTPERF_NCOUNTERS; ++i) {
	if (i != 0) output_char(spec_sep ? sep : ',', 1);
	output_int(after[i]-before[i], 1);
      }
      output_char('\n', 1);
    }
    break;
  case 1:
    // output raw
    for (i = 0; i < MTPERF_NCOUNTERS; ++i) {
      output_int(after[i]-before[i], 1);
      output_char('\n', 1);
    }
    break;
  case 2:
    // output Fibre
    output_char('[', 1);
    output_char('0', 1);
    output_char(',', 1);
    output_int(MTPERF_NCOUNTERS, 1);
    output_char(':', 1);
    for (i = 0; i < MTPERF_NCOUNTERS; ++i) {
      output_char(' ', 1);
      output_int(after[i]-before[i], 1);
    }
    output_char(']', 1);
    output_char('\n', 1);
    break;
  }
}
