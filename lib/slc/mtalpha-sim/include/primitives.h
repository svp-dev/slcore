#ifndef SVP_PRIMITIVES_H
#define SVP_PRIMITIVES_H

#define __write1(C) __asm__ __volatile__("print %0, 1" : : "r"(C))
#define __abort() while(1) __asm__ __volatile__(".int 0x74000000")
#define __nop() __asm__ __volatile__("nop\n\tswch")

#endif
