#include <stdlib.h>
#include <svp/mgsim.h>

void _exit(int status)
{
    mgsim_control(status & 0xff, MGSCTL_TYPE_STATACTION, MGSCTL_SA_EXIT, 0);
    while(1);
}

void _Exit(int status)
{
    _exit(status);
}

void exit(int status)
{
    // FIXME: some cleanup needed here.

    _exit(status);
}
