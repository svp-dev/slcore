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
      output_char('\n', stream);
    }
    break;
  case 1:
    // output raw
    for (i = 0; i < MTPERF_NCOUNTERS; ++i) {
      output_int(after[i]-before[i], stream);
      output_char('\n', stream);
    }
    break;
  case 2:
    // output Fibre
    output_char('[', stream);
    output_char('0', stream);
    output_char(',', stream);
    output_int(MTPERF_NCOUNTERS, stream);
    output_char(':', stream);
    for (i = 0; i < MTPERF_NCOUNTERS; ++i) {
      output_char(' ', stream);
      output_int(after[i]-before[i], stream);
    }
    output_char(']', stream);
    output_char('\n', stream);
    break;
  }
}
