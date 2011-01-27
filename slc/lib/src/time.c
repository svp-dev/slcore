#include <svp/perf.h>

time_t time(time_t *p)
{
    time_t c = mtperf_sample1(MTPERF_UNIX_TIME);
    if (p)
        *p = c;
    return c;
}
