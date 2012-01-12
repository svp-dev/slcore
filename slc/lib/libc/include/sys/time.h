#ifndef SLC_MTA_SYS_TIME_H
#define SLC_MTA_SYS_TIME_H

#include <sys/types.h>

struct timeval {
    time_t  tv_sec;       /* seconds */
    suseconds_t tv_usec;  /* microseconds */
};

struct timezone {
    int     tz_minuteswest; /* minutes west of Greenwich */
    int     tz_dsttime;     /* type of dst correction */
};

int     gettimeofday(struct timeval *, struct timezone *);
int     gettimeofday_virtual(struct timeval *, struct timezone *);
#ifdef __slc_os_sim__
int     gettimeofday_mgsim(struct timeval *, struct timezone *);
#endif

#if defined(MG_FAST_TIME) && defined(__slc_os_sim__)
#define gettimeofday gettimeofday_mgsim
#elif defined(MG_VIRTUAL_TIME)
#define gettimeofday gettimeofday_virtual
#endif

#endif
