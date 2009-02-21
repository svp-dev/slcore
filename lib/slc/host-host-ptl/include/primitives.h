#ifndef SVP_PRIMITIVES_H
#define SVP_PRIMITIVES_H

#include <iostream>
#include <cstdlib>

#define __write1(C) do { std::cout.put(C); std::cout.flush(); } while(0)
#define __abort() std::abort()
#define __nop() __asm__ __volatile__("/* NO OP */")

#endif
