//
// benchca.c: this file is part of the SL program suite.
//
// Copyright (C) 2011 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include <svp/gfx.h>
#include <svp/testoutput.h>
#include <svp/fibre.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"
#include "ca2d.h"

struct bdata {
    size_t icount;

    size_t space_w, space_h;
    cell *space1;
    cell *space2;
    
    size_t init_w, init_h;
    cell *init;

    rule2D rule;

    size_t block;
};


sl_def(initialize, void,
       sl_glparm(struct benchmark_state*, st))
{
    struct bdata *bdata = (struct bdata*) malloc(sizeof(struct bdata));

    size_t w = 30;
    size_t h = 30;
    size_t block = 0;
    size_t bc_s, sc_s;
    const unsigned long *birthcounts;
    const unsigned long *survivalcounts;

    size_t init_w, init_h;
    const unsigned long *init;

    const char *s;
    unsigned long v;
    if ((s = getenv("MG_BLOCK_FACTOR")) && (v = strtoul(s, 0, 10)))
        block = v;

    bdata->icount = 10;

    if (fibre_tag(0) == 0 && fibre_rank(0) == 2 && fibre_tag(1) == -1)
    {
        /* only one input 2D array, get defaults from env */
        if ((s = getenv("CA_WIDTH")) && (v = strtoul(s, 0, 10)))
            w = v;
        if ((s = getenv("CA_HEIGHT")) && (v = strtoul(s, 0, 10)))
            h = v;
        if ((s = getenv("CA_ITERATIONS")) && (v = strtoul(s, 0, 10)))
            bdata->icount = v;
        if ((s = getenv("CA_RULE_B")))
        {
            unsigned long *bp = (unsigned long*) calloc(sizeof(unsigned long), strlen(s));
            birthcounts = bp;
            assert(birthcounts != 0);
            for (const char *p = s; *p; ++p)
                if (*p >= '0' && *p <= '9')
                    *bp++ = *p - '0';
            bc_s = bp - birthcounts;
        } else {
            static const unsigned long _bc[] = { 3 };
            birthcounts = _bc;
            bc_s = 1;
        }                   
        if ((s = getenv("CA_RULE_S")))
        {
            unsigned long *bp = (unsigned long*)calloc(sizeof(unsigned long), strlen(s));
            survivalcounts = bp;
            assert(survivalcounts != 0);
            for (const char *p = s; *p; ++p)
                if (*p >= '0' && *p <= '9')
                    *bp++ = *p - '0';
            sc_s = bp - survivalcounts;
        } else {
            static const unsigned long _sc[] = { 2,3 };
            survivalcounts = _sc;
            sc_s = 2;
        }

        init_h = fibre_shape(0)[0];
        init_w = fibre_shape(0)[1];

        init = (unsigned long*)fibre_data(0);
    }
    else
    {
        assert(fibre_tag(0) == 0 && fibre_rank(0) == 0);
        bdata->icount = *(unsigned long*)fibre_data(0);

        assert(fibre_tag(1) == 0 && fibre_rank(1) == 0);
        w = *(unsigned long*)fibre_data(1);
        assert(w > 0);

        assert(fibre_tag(2) == 0 && fibre_rank(2) == 0);
        h = *(unsigned long*)fibre_data(2);
        assert(h > 0);

        assert(fibre_tag(3) == 0 && fibre_rank(3) == 1);
        birthcounts = (unsigned long*)fibre_data(3);
        bc_s = fibre_shape(3)[0];

        assert(fibre_tag(4) == 0 && fibre_rank(4) == 1);
        survivalcounts = (unsigned long*)fibre_data(4);
        sc_s = fibre_shape(4)[0];

        assert(fibre_tag(5) == 0 && fibre_rank(5) == 2);
        init_h = fibre_shape(5)[0];
        init_w = fibre_shape(5)[1];

        init = (unsigned long*)fibre_data(5);
    }

    size_t sp_w = w + 2;
    size_t sp_h = h + 2;

    /* load rule */
    makerule(&bdata->rule, birthcounts, bc_s, survivalcounts, sc_s);
    output_string("CA Rule:\n", 2);
    for (size_t i = 0; i < sizeof(bdata->rule)/sizeof(bdata->rule[0]); ++i)
        output_hex(bdata->rule[i], 2);
    output_char('\n', 2);
    
    /* init space */
    bdata->space1 = (cell*)malloc(sp_w * sp_h * sizeof(cell));
    assert(bdata->space1 != 0);
    bdata->space2 = (cell*)malloc(sp_w * sp_h * sizeof(cell));
    assert(bdata->space2 != 0);

    cell (*cinit)[init_h][init_w] = (cell (*)[init_h][init_w])malloc(init_w * init_h * sizeof(cell));

    for (size_t j = 0; j < init_h; ++j)
        for (size_t i = 0; i < init_w; ++i)
            (*cinit)[j][i] = *(init + j * init_w + i);

#ifdef DISPLAY_DURING_COMPUTE
    gfx_init();
    gfx_resize(sp_w, sp_h);
#endif
    
    bdata->space_w = sp_w;
    bdata->space_h = sp_h;
    bdata->init_w = init_w;
    bdata->init_h = init_h;
    bdata->init = (cell*)cinit;
    bdata->block = block;

    sl_getp(st)->data = bdata;
}            
sl_enddef

sl_def(prepare, void,
       sl_glparm(struct benchmark_state*, st))
{
    struct bdata *bdata = (struct bdata*)sl_getp(st)->data;
    
    /* copy initial state */
    size_t init_h = bdata->init_h;
    size_t init_w = bdata->init_w;
    size_t sp_w = bdata->space_w;
    size_t sp_h = bdata->space_h;

    memset(bdata->space1, 0, sp_w * sp_h * sizeof(cell));

    cell (*space1)[sp_h][sp_w] = (cell (*)[sp_h][sp_w])bdata->space1;

    cell (*init)[init_h][init_w] = 
        (cell (*)[init_h][init_w]) bdata->init;
#ifdef CENTER_INIT_PATTERN
    size_t base_i = ((bdata->space_w - 2) / 2) - (init_w / 2);
    size_t base_j = ((bdata->space_h - 2) / 2) - (init_h / 2);
#else
    size_t base_i = 0;
    size_t base_j = 0;
#endif

    for (size_t j = 0; j < init_h; ++j)
        for (size_t i = 0; i < init_w; ++i)
        {
            bool v = (*init)[j][i];
            // printf("Init: %zux%zu = %d\n", i, j, v);
            (*space1)[j+1+base_j][i+1+base_i] = v;
#ifdef DISPLAY_DURING_COMPUTE
            gfx_fb_set(sp_w*(j+1+base_j)+i+1+base_i, v ? -1 : 0);
#endif
        }
}
sl_enddef

sl_def(work, void,
       sl_glparm(struct benchmark_state*, st))
{
    struct bdata *bdata = (struct bdata*)sl_getp(st)->data;
    
    cell *space1 = (cell*)bdata->space1;
    cell *space2 = (cell*)bdata->space2;

    for (size_t i = 0; i < bdata->icount; ++i)
    {
        sl_create(,,,,,,, cycle2D,
                  sl_glarg(size_t,,bdata->space_w),
                  sl_glarg(size_t,,bdata->space_h),
                  sl_glarg(cell*,, space1),
                  sl_glarg(cell*,, space2),
                  sl_glarg(rule2D*,,  &bdata->rule),
                  sl_glarg(size_t,, bdata->block));
        sl_sync();

        /* swap spaces for next iteration */
        cell * t = space1;
        space1 = space2;
        space2 = t;
    }
}
sl_enddef

sl_def(output, void,
       sl_glparm(struct benchmark_state*, st))
{
    struct bdata *bdata = (struct bdata*)sl_getp(st)->data;

    size_t sp_w = bdata->space_w;
    size_t sp_h = bdata->space_h;

    cell (*space1)[sp_h][sp_w] = (cell (*)[sp_h][sp_w])bdata->space1;

    gfx_resize(sp_w - 2, sp_h - 2);

    for (size_t j = 1; j < sp_h-1; ++j)
        for (size_t i = 1; i < sp_w-1; ++i)
            gfx_putpixel(i-1,j-1, (*space1)[j][i] ? -1 : 0);
    
    /* dump screenshot */
    gfx_dump(0, 1, 0, 0);    
}
sl_enddef

sl_def(teardown, void,
       sl_glparm(struct benchmark_state*, st))
{
#ifdef DISPLAY_DURING_COMPUTE
  gfx_close();
#endif
  struct bdata *bdata = (struct bdata*)sl_getp(st)->data;

  free(bdata->space1);
  free(bdata->space2);
  free(bdata->init);
  free(bdata);
}
sl_enddef

sl_def(t_main, void)
{
  struct benchmark b = {
    "Cellular automaton",
    "kena",
    "Iterate a generic CA over a 2D torus",
    &initialize, &prepare, &work, &output, &teardown
  };
  sl_proccall(run_benchmark, sl_glarg(struct benchmark*, , &b));

}
sl_enddef

#include "makerule2d.c"
#include "ca2d.c"
