//
// mandel.c: this file is part of the SL program suite.
//
// Copyright (C) 2009,2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include <svp/compiler.h>
#include <svp/testoutput.h>
#include <svp/fibre.h>
#include <svp/gfx.h>
#include <svp/sep.h>
#include <cassert.h>
#include <cstdlib.h>
#include <cmath.h>

#include "benchmark.h"

struct point
{ short x, y; uint32_t data; };

struct bdata {
  /* input parameters */
  uint16_t xN, yN;
  uint16_t icount;
  uint16_t blocksize;
  double xmin, xmax, ymin, ymax;
  /* computed at initialization */
  double xstep, ystep;
  size_t N;
  sl_place_t par_place;
  sl_place_t excl_place;
#ifdef MANY_COLORS
  uint32_t * restrict colors;
#endif
#ifndef SKIP_MEM
  struct point * restrict pixeldata;
#endif
#if SVP_HAS_SEP
  struct placeinfo *p1;
  struct placeinfo *p2;
#endif
};

sl_def(prepare_colors, void,
       sl_glparm(uint32_t*restrict, colors),
       sl_glfparm(double, licount))
{
  sl_index(i);

#define RGB(G) ((G << 16) | (G << 8) | 128+(G/2))
  uint32_t v = 255*log(1+i)/sl_getp(licount);
  sl_getp(colors)[i] = RGB(v);
}
sl_enddef

sl_def(initialize, void,
       sl_glparm(struct benchmark_state*, st))
{
  struct bdata *bdata = (struct bdata*) malloc(sizeof(struct bdata));

  /* fibre input:
     4 x ulong (xres, yres, icount, blocksize)
     1 x double array (box)
  */
  assert(fibre_tag(0) == 0);
  assert(fibre_rank(0) == 0);
  bdata->xN = *(unsigned long*)fibre_data(0);

  assert(fibre_tag(1) == 0);
  assert(fibre_rank(1) == 0);
  bdata->yN = *(unsigned long*)fibre_data(1);

  assert(fibre_tag(2) == 0);
  assert(fibre_rank(2) == 0);
  bdata->icount = *(unsigned long*)fibre_data(2);
  assert(bdata->icount > 0);

  assert(fibre_tag(3) == 0);
  assert(fibre_rank(3) == 0);
  bdata->blocksize = *(unsigned long*)fibre_data(3);

  assert(fibre_tag(4) == 2);
  assert(fibre_rank(4) == 1);
  assert(fibre_shape(4)[0] == 4);
  double *box = (double*)fibre_data(4);
  bdata->xmin = box[0];
  bdata->xmax = box[1];
  bdata->ymin = box[2];
  bdata->ymax = box[3];

  bdata->xstep = (bdata->xmax - bdata->xmin) / bdata->xN;
  bdata->ystep = (bdata->ymax - bdata->ymin) / bdata->yN;

  bdata->N = bdata->xN * bdata->yN;
  assert(bdata->N > 0);

#ifndef SKIP_MEM
  bdata->pixeldata = (struct point*)malloc(sizeof(struct point)*bdata->N);
#endif

#ifdef MANY_COLORS
  /* initialize colors */
  bdata->colors = (uint32_t*)malloc(sizeof(uint32_t) * (bdata->icount + 1));
  double licount = log(bdata->icount+1);
  sl_create(,,,bdata->icount+2,,4,, prepare_colors,
	    sl_glarg(uint32_t*, , bdata->colors),
	    sl_glfarg(double, , licount));
  sl_sync();
#endif

#if SVP_HAS_SEP
  unsigned ncores_wanted = sl_getp(st)->place->ncores;
  sl_create(,root_sep->sep_place|1,,,,,, root_sep->sep_alloc,
	    sl_glarg(struct SEP*, , root_sep),
	    sl_glarg(unsigned long, , SAL_EXACT|ncores_wanted),
	    sl_sharg(struct placeinfo*, p1, 0));
  sl_sync();
  sl_create(,root_sep->sep_place|1,,,,,, root_sep->sep_alloc,
	    sl_glarg(struct SEP*, , root_sep),
	    sl_glarg(unsigned long, , SAL_DONTCARE),
	    sl_sharg(struct placeinfo*, p2, 0));
  sl_sync();
  assert(sl_geta(p1) != 0 && sl_geta(p2) != 0);

  bdata->p1 = sl_geta(p1);
  bdata->p2 = sl_geta(p2);
  bdata->par_place = sl_geta(p1)->pid;
  bdata->excl_place = sl_geta(p2)->pid|1; // FIXME: manual exclusion bit
#else
  bdata->par_place = bdata->excl_place = PLACE_DEFAULT;
#endif

  /* initialize graphics output */
  gfx_init();
  gfx_resize(bdata->xN, bdata->yN);

  sl_getp(st)->data = bdata;
}
sl_enddef


alwaysinline
void do_display(uint16_t dx, uint16_t dy, uint32_t v)
{
#ifdef TEXT
  output_uint(dx, 1);
  output_char(' ', 1);
  output_uint(dy, 1);
  output_char(' ', 1);
  output_hex(v, 1);
  output_char('\n', 1);
#endif
  gfx_putpixel(dx, dy, v);
}


sl_def(displayPoint, void,
       sl_glparm(uint16_t, dx),
       sl_glparm(uint16_t, dy),
       sl_glparm(uint32_t, v))
{
  do_display(sl_getp(dx), sl_getp(dy), sl_getp(v));
}
sl_enddef

sl_def(displayAfter, void,
       sl_glparm(struct point*restrict, v)
#ifndef PARALLEL_DISPLAY
       , sl_shparm(int, token)
#endif
)
{
  sl_index(i);
  uint16_t x, y; uint32_t v;
  x = sl_getp(v)[i].x;
  y = sl_getp(v)[i].y;
  v = sl_getp(v)[i].data;
#ifndef PARALLEL_DISPLAY
  int tok = sl_getp(token);
#endif
  do_display(x, y, v);
#ifndef PARALLEL_DISPLAY
  sl_setp(token, tok);
#endif
}
sl_enddef

sl_def(mandel, void,
       sl_glfparm(double, four),
       sl_glfparm(double, xstart),
       sl_glfparm(double, ystart),
       sl_glfparm(double, xstep),
       sl_glfparm(double, ystep),
       sl_glparm(uint16_t, xres),
       sl_glparm(size_t, icount)
#ifdef MANY_COLORS
       , sl_glparm(uint32_t*restrict, colors)
#endif
#ifndef SKIP_MEM
       , sl_glparm(struct point*restrict, mem)
#endif
)
{
  sl_index(i);

  uint16_t  xb = i % sl_getp(xres);
  double cx = sl_getp(xstart) + xb * sl_getp(xstep);
  uint16_t  yb = i / sl_getp(xres);
  double cy = sl_getp(ystart) + yb * sl_getp(ystep);

  uint16_t dx = xb;
  uint16_t dy = yb;
#ifdef TRACE_COMPUTE
  gfx_putpixel(dx, dy, 0xff0000);
#endif

  double zx = cx, zy = cy;
  size_t v;
  // size_t ic = sl_getp(icount);
  for (v = 0; likely(v < sl_getp(icount)); ++v)
    {
        double q1 = zx * zx;
        double q2 = zy * zy;
        if (unlikely((q1 + q2) >= sl_getp(four)))
            break;
        double t = q1 - q2 + cx;
        double q3 = zx * zy;
        zy = q3 + q3 + cy;
        zx = t;
    }

#ifdef MANY_COLORS
  v = sl_getp(colors)[v];
#endif
#ifndef SKIP_MEM
  sl_getp(mem)[i].x = dx;
  sl_getp(mem)[i].y = dy;
  sl_getp(mem)[i].data = v;
#endif
#ifdef DISPLAY_DURING_COMPUTE
#ifndef PARALLEL_DISPLAY
  sl_create(,excl_place,,,,,,
	    displayPoint,
	    sl_glarg(uint16_t, , dx),
	    sl_glarg(uint16_t, , dy),
	    sl_glarg(uint32_t, , v));
  sl_sync();
#else
  do_display(dx, dy, v);
#endif
#endif
}
sl_enddef

sl_def(work, void, sl_glparm(struct benchmark_state*, st))
{
  struct work_lapses * wl = sl_getp(st)->wl;
  struct bdata *bdata = (struct bdata*)sl_getp(st)->data;

  start_interval(wl, "compute");
  sl_create(,bdata->par_place,,bdata->N,,bdata->blocksize,,
	    mandel,
            sl_glfarg(double, , 4.0),
	    sl_glfarg(double, , bdata->xmin),
	    sl_glfarg(double, , bdata->ymin),
	    sl_glfarg(double, , bdata->xstep),
	    sl_glfarg(double, , bdata->ystep),
	    sl_glarg(uint16_t, , bdata->xN),
	    sl_glarg(size_t, , bdata->icount)
#ifdef MANY_COLORS
	    , sl_glarg(uint32_t*restrict, , bdata->colors)
#endif
#ifndef SKIP_MEM
	    , sl_glarg(struct point*restrict, , bdata->pixeldata)
#endif
	    );
  sl_sync();
  finish_interval(wl);

#ifndef DISPLAY_DURING_COMPUTE
  start_interval(wl, "display");
#ifdef PARALLEL_DISPLAY
  sl_create(,bdata->par_place,,bdata->N,,bdata->blocksize,,
	    displayAfter,
	    sl_glarg(struct point*restrict, , bdata->pixeldata));
  sl_sync();
#else
  sl_create(,bdata->par_place,,bdata->N,,bdata->blocksize,,
	    displayAfter,
	    sl_glarg(struct point*restrict, , bdata->pixeldata),
	    sl_sharg(int, tok, 0));
  sl_sync();
#endif
  finish_interval(wl);

#endif
}
sl_enddef

sl_def(output, void,
       sl_glparm(struct benchmark_state*, st))
{
  /* dump screenshot */
  gfx_dump(0, 1, 0, 0);
}
sl_enddef

sl_def(teardown, void,
       sl_glparm(struct benchmark_state*, st))
{
  gfx_close();

  struct bdata *bdata = (struct bdata*)sl_getp(st)->data;
#ifndef SKIP_MEM
  free(bdata->pixeldata);
#endif
#ifdef MANY_COLORS
  free(bdata->colors);
#endif
#if SVP_HAS_SEP
  sl_create(,root_sep->sep_place|1,,,,,, root_sep->sep_free,
	    sl_glarg(struct SEP*, _s1, root_sep),
	    sl_glarg(struct placeinfo*, _s2, bdata->p1));
  sl_sync();
  sl_create(,root_sep->sep_place|1,,,,,, root_sep->sep_free,
	    sl_glarg(struct SEP*, _s3, root_sep),
	    sl_glarg(struct placeinfo*, _s4, bdata->p2));
  sl_sync();
#endif
  free(bdata);
}
sl_enddef

sl_def(t_main, void)
{
  struct benchmark b = {
    "Mandelbrot set approximation",
    "kena",
    "Iterate z_{n-1} = z_n^2 + c over the complex plane",
    &initialize, 0, &work, &output, &teardown
  };
  sl_proccall(run_benchmark, sl_glarg(struct benchmark*, b, &b));

}
sl_enddef
