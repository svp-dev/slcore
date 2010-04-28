//
// perf.c: this file is part of the SL toolchain.
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
#include <cstdio.h>

// XIGNORE: *:D

sl_def(t_main, void)
{
  counter_t ct[2][MTPERF_NCOUNTERS];
  mtperf_sample(ct[0]);
  puts("hello, world!");
  mtperf_sample(ct[1]);

  puts("differences (raw):");
  mtperf_report_diffs(ct[0], ct[1], REPORT_RAW);
  puts("differences (Fibre):");
  mtperf_report_diffs(ct[0], ct[1], REPORT_FIBRE);
  puts("differences (CSV):");
  mtperf_report_diffs(ct[0], ct[1], REPORT_CSV);
  puts("differences (CSV+header):");
  mtperf_report_diffs(ct[0], ct[1], REPORT_CSV|CSV_INCLUDE_HEADER);
  puts("differences (CSV+tabs):");
  mtperf_report_diffs(ct[0], ct[1], REPORT_CSV|CSV_SEP('\t'));
}
sl_enddef
