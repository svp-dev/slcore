//
// xdot.c: this file is part of the SL toolchain.
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

#include <cmath.h>

#define INT long

sl_def(FUNCTION[[]]_mt, void,
       sl_glparm(FLOAT*restrict, sx),
       sl_shfparm(FLOAT, res))
{
  sl_index(i);
  FLOAT *restrict lsx = sl_getp(sx) + i;
  sl_setp(res, *lsx * *lsx + sl_getp(res));
}
sl_enddef

sl_def(FUNCTION, void,
       sl_shfparm(FLOAT, res),
       sl_glparm(INT, n),
       sl_glparm(FLOAT*, sx),
       sl_glparm(INT, incx))
{
  INT sx = (sl_getp(incx) < 0) ? ((-sl_getp(n) + 1) * sl_getp(incx)) : 0;
  INT lx = (sl_getp(incx) < 0) ? -1 : sl_getp(n);

  sl_create(,, sx, lx, sl_getp(incx),,,
	    FUNCTION[[]]_mt,
	    sl_glarg(FLOAT*, _1, sl_getp(sx)),
	    sl_shfarg(FLOAT, sres, sl_getp(res)));
  sl_sync();
  sl_setp(res, SQRT(sl_geta(sres)));
}
sl_enddef
