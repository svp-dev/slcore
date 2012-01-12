#include <sys/time.h>
#include <time.h>
#include <svp/perf.h>

time_t time_mgsim(time_t* p)
{
    time_t t = mtperf_sample1(MTPERF_UNIX_CTIME);
    if (p)
        *p = t;
    return t;
}
