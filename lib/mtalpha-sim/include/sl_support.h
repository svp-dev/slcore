#ifndef SL_SUPPORT_H
#define SL_SUPPORT_H

#include "svp_os.h"

#define __svp_break(Val) do { \
    __asm__ __volatile__("break %0" : : "rI"(Val)); \
    while(1) __nop();				    \
  } while(0);

#define __svp_breakf(Val) do { \
    __asm__ __volatile__("breakf %0" : : "r"(Val)); \
    while(1) __nop();				    \
  } while(0);

#define __svp_kill(Val) __asm__ __volatile__("kill %0" : : "r"(Val))

typedef long long sl_family_t;
typedef long long sl_place_t;

#define SVP_LOCAL 0

#define SVP_ENORMAL 0
#define SVP_EBROKEN 1
#define SVP_EKILLED 2

#endif
