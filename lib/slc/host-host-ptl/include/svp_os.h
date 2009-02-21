#ifndef SVP_OS_H
#define SVP_OS_H

#include <iostream>
#include <cstdlib>

#define __write1(C) do { std::cout.put(C); std::cout.flush(); } while(0)
#define __abort() std::abort()
#define __nop() __asm__ __volatile__("/* NO OP */")

#endif
