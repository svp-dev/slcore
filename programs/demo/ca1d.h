#ifndef CA1D_H
#define CA1D_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef bool cell;
typedef uint8_t rule1D;

sl_decl(cycle1D,,
        sl_glparm(size_t, w),
        sl_glparm(cell *, old),
        sl_glparm(cell *, new),
        sl_glparm(rule1D, rule),
        sl_glparm(size_t, block)
#ifdef DRAW
       , sl_glparm(size_t,, sl_getp(drawoffset))
#endif
); 

#endif
