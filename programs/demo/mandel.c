//
// mandel.c: this file is part of the SL toolchain.
//
// Copyright (C) 2009 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include <svp/testoutput.h>
#include <svp/iomacros.h>
#include <svp/compiler.h>
#include <svp/slr.h>
#include <svp/perf.h>
#include <svp/sep.h>
#include <svp/gfx.h>
#include <svp/assert.h>

// SLT_RUN: res=10,10 nprocs=4 -n 1,1,4

sl_place_t par_place;
sl_place_t excl_place;

slr_decl(slr_var(unsigned, res, "resolution (W,H)"),
	 slr_var(double, box, "bounding box (xmin,xmax,ymin,ymax)"),
	 slr_var(unsigned, icount, "max # iterations"),
	 slr_var(unsigned, blocksize, "# threads / core"),
	 slr_var(unsigned, nprocs, "desired number of cores"),
	 slr_var(int, verbose));


__attribute__((__always_inline__))
void do_display(unsigned dx, unsigned dy, unsigned long v)
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
       sl_glparm(unsigned, dx),
       sl_glparm(unsigned, dy),
       sl_glparm(unsigned long, v))
{
  do_display(sl_getp(dx), sl_getp(dy), sl_getp(v));
}
sl_enddef

typedef unsigned uint3[3];

sl_def(displayAfter, void,
       sl_glparm(uint3*restrict, v)
#ifndef PARALLEL_DISPLAY
       , sl_shparm(int, token)
#endif
)
{
  sl_index(i);
  unsigned x, y, v;
  x = sl_getp(v)[i][0];
  y = sl_getp(v)[i][1];
  v = sl_getp(v)[i][2];
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
       sl_glfparm(double, xstart),
       sl_glfparm(double, ystart),
       sl_glfparm(double, xstep),
       sl_glfparm(double, ystep),
       sl_glfparm(double, xrange),
       sl_glfparm(double, yrange),
       sl_glparm(unsigned, xres),
       sl_glparm(unsigned, yres),
       sl_glparm(unsigned long, icount),
       sl_glfparm(double, pscale)
#ifndef SKIP_MEM
       , sl_glparm(uint3*restrict, mem)
#endif
)
{
  sl_index(i);

  unsigned  xb = i % sl_getp(xres);
  double cx = sl_getp(xstart) + xb * sl_getp(xstep);
  unsigned  yb = i / sl_getp(xres);
  double cy = sl_getp(ystart) + yb * sl_getp(ystep);

  unsigned dx = xb;
  unsigned dy = yb;
  gfx_putpixel(dx, dy, 0xff0000);

  double zx = cx, zy = cy;
  unsigned long v;
  unsigned long ic = sl_getp(icount);
  for (v = 0; (zx*zx+zy*zy) < 4.0 && v < ic; ++v)
    {
      double t = zx * zx - zy * zy + cx;
      zy = 2 * zx * zy + cy;
      zx = t;
    }

  v *= sl_getp(pscale);
  v = (v > 255) ? 255 : v;
  v &= 0xff;
  v = (v << 16) | (v << 8) | v;
#ifndef SKIP_MEM
  sl_getp(mem)[i][0] = dx;
  sl_getp(mem)[i][1] = dy;
  sl_getp(mem)[i][2] = v;
#endif
#ifdef DISPLAY_DURING_COMPUTE
#ifndef PARALLEL_DISPLAY
  sl_create(,excl_place,,,,,,
	    displayPoint,
	    sl_glarg(unsigned, _0, dx),
	    sl_glarg(unsigned, _1, dy),
	    sl_glarg(unsigned long, _2, v));
  sl_sync();
#else
  do_display(dx, dy, v);
#endif
#endif
}
sl_enddef

sl_def(t_main, void)
{
  unsigned xN = 100, yN = 100;
  double xmin = -2., xmax = 2., ymin=-2., ymax=2.;
  unsigned long i, icount = 32;
  double pscale;
  int verbose = 0;
  unsigned nprocs_wanted = 1;

  if (slr_len(verbose) && slr_get(verbose)[0])
    verbose = 1;

  if (slr_len(nprocs))
    nprocs_wanted = slr_get(nprocs)[0];

#if SVP_HAS_SEP
  sl_create(,sep_place->pid,,,,,, sep_alloc,
	    sl_glarg(enum sep_alloc_policy, _s0, SAL_MIN),
	    sl_glarg(int, _s1, nprocs_wanted),
	    sl_sharg(struct placeinfo*, p1, 0));
  sl_sync();
  sl_create(,sep_place->pid,,,,,, sep_alloc,
	    sl_glarg(enum sep_alloc_policy, _s2, SAL_DONTCARE),
	    sl_glarg(int, _s3, 0),
	    sl_sharg(struct placeinfo*, p2, 0));
  sl_sync();
  svp_assert(sl_geta(p1) != 0 && sl_geta(p2) != 0);
  if (verbose)
    printf("# Running compute with %d cores, exclusion with %d cores\n",
	   sl_geta(p1)->ncores, sl_geta(p2)->ncores);

  par_place = sl_geta(p1)->pid;
  excl_place = sl_geta(p2)->pid | 1; // FIXME: manual exclusion bit
#else
  par_place = excl_place = PLACE_DEFAULT;
#endif

  if (slr_len(icount))
    icount = slr_get(icount)[0];
  pscale = 256./(float)icount;

  if (slr_len(res)) {
    xN = slr_get(res)[0];
    yN = slr_get(res)[1];
  }

  if (slr_len(box)) {
    xmin = slr_get(box)[0];
    xmax = slr_get(box)[1];
    ymin = slr_get(box)[2];
    ymax = slr_get(box)[3];
  }

  unsigned B = 0;
  if (slr_len(blocksize))
    B = slr_get(blocksize)[0];

  unsigned N = xN * yN;

#ifndef SKIP_MEM
  uint3 values[N];
#endif

  gfx_init();
  gfx_resize(xN, yN);

  if (verbose)
    printf("# Computing... (icount=%d)\n", icount);

  uint64_t c1 = get_cycles();
  sl_create(,par_place,,N,,B,,
	    mandel,
	    sl_glfarg(double, _0, xmin),
	    sl_glfarg(double, _1, ymin),
	    sl_glfarg(double, _2, (xmax-xmin)/xN),
	    sl_glfarg(double, _3, (ymax-ymin)/yN),
	    sl_glfarg(double, _4, (xmax-xmin)),
	    sl_glfarg(double, _5, (ymax-ymin)),
	    sl_glarg(unsigned, _6, xN),
	    sl_glarg(unsigned, _7, yN),
	    sl_glarg(unsigned long, _7a, icount),
	    sl_glfarg(double, _7b, pscale)
#ifndef SKIP_MEM
	    , sl_glarg(uint3*restrict, _8, values)
#endif
	    );
  sl_sync();
#ifndef DISPLAY_DURING_COMPUTE

#ifdef PARALLEL_DISPLAY
  sl_create(,par_place,,N,,B,,
	    displayAfter,
	    sl_glarg(uint3*restrict, _9, values));
  sl_sync();
#else
  sl_create(,par_place,,N,,B,,
	    displayAfter,
	    sl_glarg(uint3*restrict, _9, values),
	    sl_sharg(int, tok, 0));
  sl_sync();
#endif

#endif
  uint64_t c2 = get_cycles();

  if (verbose)
    printf("# Done. (%d cycles = %f s @ 1Ghz)\n",
	   c2-c1, ((float)(c2-c1))/1e9);

  gfx_dump(0, 1, 0, 0);
  gfx_close();

#if SVP_HAS_SEP
  sl_create(,sep_place->pid,,,,,, sep_free,
	    sl_glarg(struct placeinfo*, _sf0, sl_geta(p1)));
  sl_sync();
  sl_create(,sep_place->pid,,,,,, sep_free,
	    sl_glarg(struct placeinfo*, _sf1, sl_geta(p2)));
  sl_sync();
#endif
}
sl_enddef
