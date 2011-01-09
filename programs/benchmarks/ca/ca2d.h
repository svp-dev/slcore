#ifndef CA2D_H
#define CA2D_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef bool cell;
typedef uint8_t rule2D[64];

sl_decl(cycle2D,,
        sl_glparm(size_t, w),
        sl_glparm(size_t, h),
        sl_glparm(cell *, old),
        sl_glparm(cell *, new),
        sl_glparm(rule2D *, rule),
        sl_glparm(size_t, block));

void makerule(rule2D*, 
              const size_t*, size_t,
              const size_t*, size_t);

#endif
