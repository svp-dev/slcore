#include <stdlib.h>
#include <svp/testoutput.h>

#ifdef __slc_os_sim__
#include <svp/mgsim.h>
#endif

void _exit(int status)
{
#ifdef __slc_os_sim__
    mgsim_control(status & 0xff, MGSCTL_TYPE_STATACTION, MGSCTL_SA_EXIT, 0);
#elif defined(__slc_arch_leon2mt__) && defined(__slc_os_fpga__)
    __dbg_exit_status = status;
    __asm__ __volatile__("t_end");
#endif
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
