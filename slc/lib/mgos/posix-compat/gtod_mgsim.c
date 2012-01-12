#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <svp/perf.h>

#include "mgsim/mtconf.h"

int gettimeofday_mgsim(struct timeval* tv, struct timezone *tz)
{
    if (tz)
    {
        // we don't support timezones for now.
        errno = EINVAL;
        return -1;
    }

    time_t c = mtperf_sample1(MTPERF_UNIX_CTIME);
    counter_t dtime = mtperf_sample1(MTPERF_UNIX_PACKED_TIME);
    tv->tv_sec = c;
    tv->tv_usec = (dtime & 0x7fff) << 5;

    return 0;
}

