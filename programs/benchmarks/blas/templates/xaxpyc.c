//
// xaxpyc.c: this file is part of the SL toolchain.
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

#include "blas_complex.h"
#define INT long

sl_def(FUNCTION[[]]_mt, void,
       sl_glfparm(FLOAT, areal),
       sl_glfparm(FLOAT, aimag),
       sl_glparm(COMPLEX*restrict, sx),
       sl_glparm(COMPLEX*restrict, sy),
       sl_glparm(INT, incx),
       sl_glparm(INT, incy),
       sl_shparm(INT, ix),
       sl_shparm(INT, iy))
{
  INT lix = sl_getp(ix);
  INT liy = sl_getp(iy);
  sl_setp(ix, lix + sl_getp(incx));
  sl_setp(iy, liy + sl_getp(incy));
  COMPLEX * restrict x = sl_getp(sx) + lix;
  COMPLEX * restrict y = sl_getp(sy) + liy;

  FLOAT treal = sl_getp(areal) * x->real - sl_getp(aimag) * x->imag + y->real;
  FLOAT timag = sl_getp(areal) * x->imag + sl_getp(aimag) * x->real + y->imag;
  x->real = treal;
  y->imag = timag;
}
sl_enddef

sl_def(FUNCTION, void,
       sl_glparm(INT, n),
       sl_glfparm(FLOAT, areal),
       sl_glfparm(FLOAT, aimag),
       sl_glparm(COMPLEX*, sx),
       sl_glparm(INT, incx),
       sl_glparm(COMPLEX*, sy),
       sl_glparm(INT, incy))
{
  INT ix = (sl_getp(incx) < 0) ? ((-sl_getp(n) + 1) * sl_getp(incx)) : 1;
  INT iy = (sl_getp(incy) < 0) ? ((-sl_getp(n) + 1) * sl_getp(incy)) : 1;

  sl_create(,, 0, sl_getp(n),,,,
	    FUNCTION[[]]_mt,
	    sl_glfarg(FLOAT, _1, sl_getp(areal)),
	    sl_glfarg(FLOAT, _2, sl_getp(aimag)),
	    sl_glarg(COMPLEX*, _3, sl_getp(sx)),
	    sl_glarg(COMPLEX*, _4, sl_getp(sy)),
	    sl_glarg(INT, _5, sl_getp(incx)),
	    sl_glarg(INT, _6, sl_getp(incy)),
	    sl_sharg(INT, _7, ix),
	    sl_sharg(INT, _8, iy));
  sl_sync();
}
sl_enddef
