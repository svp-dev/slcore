#ifndef SVP_OS_H
#define SVP_OS_H

#define __write1(C) __asm__ __volatile__("print %0, 1" : : "r"(C))
#define __abort() while(1) __asm__ __volatile__("nop\n\tnop\n\tpal1d 0")
#define __nop() __asm__ __volatile__("nop\n\tswch")

#endif
