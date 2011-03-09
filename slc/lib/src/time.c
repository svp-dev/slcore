//
// time.c: this file is part of the SL toolchain.
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
#include <time.h>

time_t time(time_t *p)
{
    time_t c = mtperf_sample1(MTPERF_UNIX_TIME);
    if (p)
        *p = c;
    return c;
}
