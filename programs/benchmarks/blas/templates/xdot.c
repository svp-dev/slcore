//
// xdot.sl: this file is part of the slc project.
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
// $Id$
//

#define INT long

sl_def(FUNCTION[[]]_mt, void,
       sl_glparm(FLOAT*restrict, sx),
       sl_glparm(FLOAT*restrict, sy),
       sl_glparm(INT, incx),
       sl_glparm(INT, incy),
       sl_shparm(INT, ix),
       sl_shparm(INT, iy),
       sl_shfparm(FLOAT, res))
{
  INT lix = sl_getp(ix);
  INT liy = sl_getp(iy);
  sl_setp(ix, lix + sl_getp(incx));
  sl_setp(iy, liy + sl_getp(incy));
  sl_setp(res, sl_getp(res) + *(sl_getp(sx) + lix) * *(sl_getp(sy) + liy));
}
sl_enddef

sl_def(FUNCTION, void,
       sl_shfparm(FLOAT, res),
       sl_glparm(INT, n),
       sl_glparm(FLOAT*, sx),
       sl_glparm(INT, incx),
       sl_glparm(FLOAT*, sy),
       sl_glparm(INT, incy))
{
  INT ix = (sl_getp(incx) < 0) ? ((-sl_getp(n) + 1) * sl_getp(incx)) : 1;
  INT iy = (sl_getp(incy) < 0) ? ((-sl_getp(n) + 1) * sl_getp(incy)) : 1;

  sl_create(,, 0, sl_getp(n),,,,
	    FUNCTION[[]]_mt,
	    sl_glarg(FLOAT*, _1, sl_getp(sx)),
	    sl_glarg(FLOAT*, _2, sl_getp(sy)),
	    sl_glarg(INT, _3, sl_getp(incx)),
	    sl_glarg(INT, _4, sl_getp(incy)),
	    sl_sharg(INT, _5, ix),
	    sl_sharg(INT, _6, iy),
	    sl_shfarg(FLOAT, sres, 0));
  sl_sync();
  sl_setp(res, sl_geta(sres));
}
sl_enddef
