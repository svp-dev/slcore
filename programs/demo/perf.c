//
// perf.c: this file is part of the SL program suite.
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

// XIGNORE: *:D
sl_def(t_main, void)
{
    counter_t a[MTPERF_NCOUNTERS];
    counter_t b[MTPERF_NCOUNTERS];
    mtperf_sample(b);
    mtperf_sample(a);
    mtperf_report_diffs(b, a, REPORT_CSV | CSV_INCLUDE_HEADER | CSV_SEP(' '));
}
sl_enddef
