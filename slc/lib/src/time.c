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
#include "mtconf.h"

static
time_t rtc_gettime(void)
{
    if (mg_rtc_devid == (confword_t)-1)
        return 1;

    volatile const uint32_t* rtc = (const uint32_t*)mg_devinfo.base_addrs[mg_rtc_devid];
    return rtc[5];
}

sl_def(get_time, sl__static, sl_shparm(time_t, t))
{
    sl_setp(t, rtc_gettime());
}
sl_enddef

time_t time(time_t *p)
{
    time_t c;
#if defined(__slc_os_fpga__)
    // UTLEON3 does not support suspending allocate,
    // but it does not matter because we are single core
    c = rtc_gettime();
#else
    sl_create(, 1,,,,,sl__forcewait, get_time, sl_sharg(time_t, c, 0));
    sl_sync();
    c = sl_geta(c);
#endif
    if (p)
        *p = c;
    return c;
}
