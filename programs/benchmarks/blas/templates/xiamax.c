//
// xiamax.c: this file is part of the SL program suite.
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
#include <svp/compiler.h>

#define INT long

sl_def(FUNCTION[[]]_mt, void,
       sl_glparm(const FLOAT*restrict, sx),
       sl_shparm(INT, idx),
       sl_shfparm(FLOAT, max))
{
  sl_index(i);
  const FLOAT *restrict lsx = sl_getp(sx) + i;
  FLOAT t = ABS(*lsx);
  if (t <= sl_getp(max)) {
    sl_setp(idx, sl_getp(idx));
    sl_setp(max, sl_getp(max));
  } else {
    INT dummy = sl_getp(idx); use(dummy);
    sl_setp(idx, i);
    sl_setp(max, t);
  }
}
sl_enddef

sl_def(FUNCTION, void,
       sl_shparm(INT, res),
       sl_glparm(INT, n),
       sl_glparm(const FLOAT*, sx),
       sl_glparm(INT, incx))
{
  INT sx = (sl_getp(incx) < 0) ? ((-sl_getp(n) + 1) * sl_getp(incx)) : 0;
  INT lx = (sl_getp(incx) < 0) ? -1 : sl_getp(n);

  sl_create(,, sx, lx, sl_getp(incx),,,
	    FUNCTION[[]]_mt,
	    sl_glarg(const FLOAT*, , sl_getp(sx)),
	    sl_sharg(INT, sres, -1),
	    sl_shfarg(FLOAT, , sl_getp(res)));
  sl_sync();
  sl_setp(res, sl_geta(sres) + 1);
}
sl_enddef
