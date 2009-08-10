//
// xaxpy.c: this file is part of the SL toolchain.
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

#define INT long

sl_def(FUNCTION[[]]_mt, void,
       sl_glfparm(FLOAT, a),
       sl_glparm(FLOAT*restrict, sx),
       sl_glparm(FLOAT*restrict, sy),
       sl_glparm(INT, incx),
       sl_glparm(INT, incy),
       sl_glparm(INT, ix),
       sl_glparm(INT, iy))
{
  sl_index(i);
  FLOAT *restrict lsx = sl_getp(sx) + sl_getp(ix) + i * sl_getp(incx);
  FLOAT *restrict lsy = sl_getp(sy) + sl_getp(iy) + i * sl_getp(incy);
  *lsy = sl_getp(a) * *lsx + *lsy;
}
sl_enddef

sl_def(FUNCTION, void,
       sl_glparm(INT, n),
       sl_glfparm(FLOAT, a),
       sl_glparm(FLOAT*, sx),
       sl_glparm(INT, incx),
       sl_glparm(FLOAT*, sy),
       sl_glparm(INT, incy))
{
  INT ix = (sl_getp(incx) < 0) ? ((-sl_getp(n) + 1) * sl_getp(incx)) : 0;
  INT iy = (sl_getp(incy) < 0) ? ((-sl_getp(n) + 1) * sl_getp(incy)) : 0;

  sl_create(,, 0, sl_getp(n),,,,
	    FUNCTION[[]]_mt,
	    sl_glfarg(FLOAT, _0, sl_getp(a)),
	    sl_glarg(FLOAT*, _1, sl_getp(sx)),
	    sl_glarg(FLOAT*, _2, sl_getp(sy)),
	    sl_glarg(INT, _3, sl_getp(incx)),
	    sl_glarg(INT, _4, sl_getp(incy)),
	    sl_glarg(INT, _5, ix),
	    sl_glarg(INT, _6, iy));
  sl_sync();
}
sl_enddef
