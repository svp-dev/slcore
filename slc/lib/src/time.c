#include <sys/time.h>
#include <time.h>
#include "mtconf.h"

time_t time(time_t* p)
{
    struct timeval tv;
    time_t ret;
    if (gettimeofday(&tv, 0) == -1)
        ret = (time_t)-1;
    else
        ret = tv.tv_sec;
    if (p)
        *p = ret;
    return ret;
}
