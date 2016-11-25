//
// gettimeofday.c: this file is part of the SL toolchain.
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
#include <sys/time.h>
#include <errno.h>

#include "mgsim/mtconf.h"

static
int rtc_gettime(struct timeval *tv)
{
    if (mg_rtc_devid == (confword_t)-1)
    {
        errno = EIO;
        return -1;
    }

    volatile const uint32_t* rtc = (const uint32_t*)mg_devinfo.base_addrs[mg_rtc_devid];
    tv->tv_sec = rtc[5];
    tv->tv_usec = rtc[4];
    return 0;
}

sl_def(get_time, sl__static, sl_glparm(struct timeval*, t), sl_shparm(int, ret))
{
    sl_setp(ret, rtc_gettime(sl_getp(t)));
}
sl_enddef

int gettimeofday(struct timeval* t, struct timezone *tz)
{
    if (tz)
    {
        // we don't support timezones for now.
        errno = EINVAL;
        return -1;
    }

#if defined(__slc_os_sim__) && (defined(__slc_arch_mtsparc__) || defined(__slc_arch_mtalpha__))
    sl_create(, mg_io_place_id,,,,,sl__forcewait, get_time, sl_glarg(struct timeval*,,t), sl_sharg(int, ret, 0));
    sl_sync();
    return sl_geta(ret);
#else
    // Single core, no bother.
    return rtc_gettime(t);
#endif
}
