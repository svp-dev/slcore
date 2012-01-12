#include <errno.h>
#include <sys/time.h>
#include <time.h>

#include "mgsim/mtconf.h"

int gettimeofday_virtual(struct timeval* tv, struct timezone *tz)
{
    if (tz)
    {
        // we don't support timezones for now.
        errno = EINVAL;
        return -1;
    }

    clock_t ticks = clock();
    unsigned long freq = (mgconf_core_freq == (confword_t)-1) ? 1000 : mgconf_core_freq;

    /* the mgsim frequencies are expressed in MHz. So it is a direct
     factor between clock ticks and microseconds. */
    unsigned long long usecs = ticks / (unsigned long long)freq;
    unsigned long secs = usecs / 1000000;

    tv->tv_sec = boot_time + secs;
    tv->tv_usec = usecs - secs;

    return 0;
}

