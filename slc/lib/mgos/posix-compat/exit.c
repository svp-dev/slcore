#include <stdlib.h>
#include <svp/testoutput.h>

#ifdef __slc_os_sim__
#include <svp/mgsim.h>
#endif

void _exit(int status)
{
#ifdef __slc_os_sim__
    mgsim_control(status & 0xff, MGSCTL_TYPE_STATACTION, MGSCTL_SA_EXIT, 0);
#elif defined(__slc_arch_leon2mt__) || defined(__slc_arch_leon2__)
    extern volatile int __exit_code;
    __exit_code = status;
    __asm__ __volatile__("b,a __stop; nop" : : "memory");
#endif
    __builtin_unreachable();
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
