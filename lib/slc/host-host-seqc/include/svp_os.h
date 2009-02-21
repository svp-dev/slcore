#ifndef SVP_OS_H
#define SVP_OS_H

#include <stdio.h>
#include <stdlib.h>

#define __write1(C) putchar(C)
#define __abort() abort()
#define __nop() __asm__ __volatile__("/* NO OP */")

#endif
