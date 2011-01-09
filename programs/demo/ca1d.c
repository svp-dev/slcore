#include "ca1d.h"
#ifdef DRAW
#include <svp/gfx.h>
#endif


sl_def(update1D, (sl__static,sl__naked(native)),
       sl_glparm(cell * restrict, old),
       sl_glparm(cell * restrict, new),
       sl_glparm(rule1D, rule)
#ifdef DRAW
       , sl_glparm(size_t, drawbase)
#endif
)
{
    sl_index(i);

#ifdef DRAW
    gfx_fb_set(sl_getp(drawbase) + i, 0xff0000);
#endif

    cell * restrict cold = sl_getp(old) + i;
    cell * restrict cnew = sl_getp(new) + i;

#define P(p, i) (*(p+i))
#define O(i,o) (P(cold, i) << o)

    size_t offset = O(-1,2) | O(0,1) | O(1,0);

    cell newval = (sl_getp(rule) >> offset) & 1;
    P(cnew, 0) = newval;
#undef P
#undef O
#ifdef DRAW
    gfx_fb_set(sl_getp(drawbase) + i, newval ? -1 : 0);
#endif
}
sl_enddef

sl_def(cycle1D,,
       sl_glparm(size_t, w),
       sl_glparm(cell *, old),
       sl_glparm(cell *, new),
       sl_glparm(rule1D, rule),
       sl_glparm(size_t, block)
#ifdef DRAW
       , sl_glparm(size_t, drawbase)
#endif
)
{
#ifdef DRAW
    gfx_fb_set(sl_getp(drawbase), 0xff);
    gfx_fb_set(sl_getp(drawbase)+sl_getp(w)+1, 0xff);
#endif
    bool v1 = sl_getp(old)[sl_getp(w)], v2 = sl_getp(old)[1];
    sl_getp(old)[0] = v1;
    sl_getp(old)[sl_getp(w)+1] = v2;
#ifdef DRAW
    gfx_fb_set(sl_getp(drawbase), v1 ? -1 : 0);
    gfx_fb_set(sl_getp(drawbase)+sl_getp(w)+1, v2 ? -1 : 0);
#endif
    sl_create(,, 1, sl_getp(w)+1,,sl_getp(block),sl__naked(native),
              update1D,
              sl_glarg(cell*,, sl_getp(old)),
              sl_glarg(cell*,, sl_getp(new)),
              sl_glarg(rule1D,, sl_getp(rule))
#ifdef DRAW
              , sl_glarg(size_t,, sl_getp(drawbase))
#endif
);
    sl_sync();
}
sl_enddef
