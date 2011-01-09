#include "ca2d.h"
#include <svp/delegate.h>
#ifdef DISPLAY_DURING_COMPUTE
#include <svp/gfx.h>
#endif

#ifdef DEBUG
#include <svp/testoutput.h>
void print_space(size_t sp_w, size_t sp_h, cell *sp_a,
                 bool print_border)
{
    cell (*sp)[sp_h][sp_w] = (cell (*)[sp_h][sp_w])sp_a;
    if (!print_border)
        for (size_t j = 1; j < sp_h - 1; ++j)
        {
            for (size_t i = 1; i < sp_w - 1; ++i)
            {
                output_char((*sp)[j][i] ? 'O' : '.', 1);
                output_char(' ', 1);
            }
            output_char('\n', 1);
        }
    else
        for (size_t j = 0; j < sp_h; ++j)
        {
            for (size_t i = 0; i < sp_w; ++i)
            {
                output_char((*sp)[j][i] ? 'O' : '.', 1);
                output_char(' ', 1);
            }
            output_char('\n', 1);
        }
}
#endif

sl_def(update2D, sl__static,
       sl_glparm(size_t, w),
       sl_glparm(cell * restrict, old),
       sl_glparm(cell * restrict, new),
       sl_glparm(rule2D * restrict, rule))
{
    sl_index(i);

#ifdef DISPLAY_DURING_COMPUTE
    gfx_fb_set(i, 0xff0000);
#endif

    cell * restrict cold = sl_getp(old) + i;
    cell * restrict cnew = sl_getp(new) + i;

#define P(p, j,i) (*(p+(j*sl_getp(w))+i))
#define O(j,i,o) (P(cold, j,i) << o)

    size_t o1 = 
        O(-1, -1, 0) | O(-1, 0, 1) | O(-1, +1, 2);
    size_t o2 =
        O(0,  -1, 0) | O(0,  0, 1) | O(0,  +1, 2) |
        O(+1, -1, 3) | O(+1, 0, 4) | O(+1, +1, 5);

    cell v = ((*sl_getp(rule))[o2] >> o1) & 1;
    P(cnew, 0,0) = v;
#undef P
#undef O
#ifdef DISPLAY_DURING_COMPUTE
    gfx_fb_set(i, v ? -1 : 0);
#endif
}
sl_enddef

sl_def(copy2D_lr, sl__static,
       sl_glparm(size_t, w),
       sl_glparm(cell * restrict, new))
{
    sl_index(j);
#ifdef DISPLAY_DURING_COMPUTE    
    gfx_fb_set(j, 0xff);
    gfx_fb_set(j+sl_getp(w)-1, 0xff);
#endif
    cell l = sl_getp(new)[j+1];
    cell r = sl_getp(new)[j+sl_getp(w)-2];

    sl_getp(new)[j] = r;
    sl_getp(new)[j+sl_getp(w)-1] = l;
#ifdef DISPLAY_DURING_COMPUTE    
    gfx_fb_set(j, r ? -1 : 0);
    gfx_fb_set(j+sl_getp(w)-1, l ? -1 : 0);
#endif
}
sl_enddef

sl_def(copy2D_tb, sl__static,
       sl_glparm(size_t, w),
       sl_glparm(cell * restrict, top),
       sl_glparm(cell * restrict, bottom))
{
    sl_index(i);

#ifdef DISPLAY_DURING_COMPUTE
    gfx_fb_set(i, 0xff);
    gfx_fb_set(i+sl_getp(top)-sl_getp(bottom)+sl_getp(w), 0xff);
#endif

    cell * restrict top = sl_getp(top) + i;
    cell * restrict bottom = sl_getp(bottom) + i;

    long b = *(long*restrict)bottom;
    long t = *(long*restrict)top;

    *(long*restrict)(bottom - sl_getp(w)) = t;
    *(long*restrict)(top + sl_getp(w)) = b;

#ifdef DISPLAY_DURING_COMPUTE
    gfx_fb_set(i, t ? -1 : 0);
    gfx_fb_set(i+sl_getp(top)-sl_getp(bottom)+sl_getp(w), b ? -1 : 0);
#endif
}
sl_enddef

sl_def(copy2D_bytes_tb, sl__static,
       sl_glparm(size_t, w),
       sl_glparm(cell * restrict, top),
       sl_glparm(cell * restrict, bottom))
{
    sl_index(i);

#ifdef DISPLAY_DURING_COMPUTE
    gfx_fb_set(i, 0xff);
    gfx_fb_set(i+sl_getp(top)-sl_getp(bottom)+sl_getp(w), 0xff);
#endif

    cell * restrict top = sl_getp(top) + i;
    cell * restrict bottom = sl_getp(bottom) + i;

    cell b = *bottom, t = *top;
    *(bottom - sl_getp(w)) = t;
    *(top + sl_getp(w)) = b;
#ifdef DISPLAY_DURING_COMPUTE
    gfx_fb_set(i, t ? -1 : 0);
    gfx_fb_set(i+sl_getp(top)-sl_getp(bottom)+sl_getp(w), b ? -1 : 0);
#endif
}
sl_enddef


sl_def(cycle2D,,
       sl_glparm(size_t, w),
       sl_glparm(size_t, h),
       sl_glparm(cell *, old),
       sl_glparm(cell *, new),
       sl_glparm(rule2D *, rule),
       sl_glparm(size_t, block))
{

#ifdef DEBUG
    output_string("Before copy:\n", 1);
    print_space(sl_getp(w), sl_getp(h), sl_getp(old), true);
#endif

    sl_create(,, sl_getp(w), sl_getp(w)*(sl_getp(h)-1), sl_getp(w),
              sl_getp(block),,
              copy2D_lr,
              sl_glarg(size_t,,sl_getp(w)),
              sl_glarg(cell*,,sl_getp(old)));
    sl_sync();

#ifdef DEBUG
    output_string("After copy LR:\n", 1);
    print_space(sl_getp(w), sl_getp(h), sl_getp(old), true);
#endif

    if ((sl_getp(w) % sizeof(long)) == 0) {
        sl_create(,, 0, sl_getp(w), sizeof(long),
                  sl_getp(block),,
                  copy2D_tb, 
                  sl_glarg(size_t,,sl_getp(w)),
                  sl_glarg(cell*,top,sl_getp(old)+sl_getp(w)*(sl_getp(h)-2)),
                  sl_glarg(cell*,bottom,sl_getp(old)+sl_getp(w)));
        sl_sync();
    } else {
        sl_create(,, 0, sl_getp(w),,
                  sl_getp(block),,
                  copy2D_bytes_tb, 
                  sl_glarg(size_t,,sl_getp(w)),
                  sl_glarg(cell*,top,sl_getp(old)+sl_getp(w)*(sl_getp(h)-2)),
                  sl_glarg(cell*,bottom,sl_getp(old)+sl_getp(w)));
        sl_sync();
    }
#ifdef DEBUG
    output_string("After copy TB:\n", 1);
    print_space(sl_getp(w), sl_getp(h), sl_getp(old), true);
#endif


    sl_create(,, sl_getp(w)+1, sl_getp(w)*(sl_getp(h)-1)-1,,
              sl_getp(block),,
              update2D, 
              sl_glarg(size_t,,sl_getp(w)),
              sl_glarg(cell*,,sl_getp(old)),
              sl_glarg(cell*,,sl_getp(new)),
              sl_glarg(rule2D*,,sl_getp(rule)));
    sl_sync();

#ifdef DEBUG
    output_string("After compute:\n", 1);
    print_space(sl_getp(w), sl_getp(h), sl_getp(new), false);
#endif

}
sl_enddef
