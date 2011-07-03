#include <errno.h>
#include <sys/time.h>
#include <time.h>

#include "mtconf.h"

#include <svp/testoutput.h>

int gettimeofday_virtual(struct timeval* tv, struct timezone *tz)
{
    if (tz)
    {
        // we don't support timezones for now.
        errno = EINVAL;
        return -1;
    }

    long long ticks = clock();
    unsigned long long freq = (mgconf_core_freq == (confword_t)-1) ? 1000 : mgconf_core_freq;
    tv->tv_usec = ticks % freq;
    tv->tv_sec = boot_time + (ticks / freq) / 1000000;
    return 0;
}

