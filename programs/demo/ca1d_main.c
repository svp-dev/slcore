#include <svp/slr.h>
#include <svp/testoutput.h>
#include <svp/perf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "ca1d.h"
#ifdef DRAW
#include <svp/gfx.h>
#endif

slr_decl(slr_var(int, rule, "Wolfram code"),
         slr_var(size_t, width, "space size"),
         slr_var(bool, init, "initial state"),
         slr_var(size_t, iters, "number of iterations"),
         slr_var(bool, print, "print output at each iteration"),
         slr_var(size_t, block, "blocking factor"));

sl_def(t_main,void)
{
    size_t w = 100;
    if (slr_len(width)) w = slr_get(width)[0];

    size_t row_width = w + 2;

    size_t iters = 100;
    if (slr_len(iters)) iters = slr_get(iters)[0];

    ++iters;

    rule1D rule = 60;
    if (slr_len(rule)) rule = slr_get(rule)[0];

    bool print = false;
    if (slr_len(print)) print = true;

    size_t block = 0;
    if (slr_len(block)) block = slr_get(block)[0];

#ifdef DRAW
    gfx_init();
    gfx_resize(row_width, iters);
#endif

    /* init space */
    cell (*rows)[iters][row_width] = (cell (*)[iters][row_width])calloc(iters, row_width);

    /* copy initial state */
    if (slr_len(init))
        for (size_t i = 0; i < slr_len(init); ++i)
        {
            bool v = (bool)slr_get(init)[i];
            (*rows)[0][1+i] = v;
#ifdef DRAW
            gfx_fb_set(1+i, v ? -1 : 0);
#endif
        }
    else
    {
        (*rows)[0][1] = true;
#ifdef DRAW
        gfx_fb_set(1, -1);
#endif
    }

    /* maybe print init */
    if (print)
    {
        for (size_t j = 1; j < w+1; ++j)
            output_char((*rows)[0][j] ? 'O' : '.', 1);
        output_char(' ', 1);
        output_uint(0, 1);
        output_char('\n', 1);
    }        

    /* iterate */

    struct s_interval iv[iters];
    mtperf_start_interval(iv, 0, 0, "all");
    for (size_t i = 1; i < iters; ++i)
    {
        mtperf_start_interval(iv, i, i, "compute");
        sl_create(,,,,,,, cycle1D,
                  sl_glarg(size_t,,w),
                  sl_glarg(cell*,, (*rows)[i-1]),
                  sl_glarg(cell*,, (*rows)[i]),
                  sl_glarg(rule1D,, rule),
                  sl_glarg(size_t,, block)
#ifdef DRAW
                  , sl_glarg(size_t,, i * row_width)
#endif
);
        sl_sync();
        mtperf_finish_interval(iv, i);

        if (print)
        {
            for (size_t j = 1; j < w+1; ++j)
                output_char((*rows)[i][j] ? 'O' : '.', 1);
            output_char(' ', 1);
            output_uint(i, 1);
            output_char('\n', 1);
        }        
    }
    mtperf_finish_interval(iv, 0);

#ifdef DRAW
    gfx_dump(0, 1, 0, 0);
    gfx_close();
#endif

    mtperf_report_intervals(iv, iters, REPORT_FIBRE|REPORT_STREAM(2));

}
sl_enddef
