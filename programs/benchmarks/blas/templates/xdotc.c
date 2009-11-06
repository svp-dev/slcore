//
// xdotc.c: this file is part of the SL toolchain.
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
       sl_glparm(COMPLEX*restrict, sx),
       sl_glparm(COMPLEX*restrict, sy),
       sl_glparm(INT, incx),
       sl_glparm(INT, incy),
       sl_shfparm(FLOAT, rreal),
       sl_shfparm(FLOAT, rimag))
{
  sl_index(i);
  COMPLEX *restrict x = sl_getp(sx) + i * sl_getp(incx);
  COMPLEX *restrict y = sl_getp(sy) + i * sl_getp(incy);

  sl_setp(rreal, sl_getp(rreal) + x->real * y->real OP1 x->imag * y->imag);
  sl_setp(rimag, sl_getp(rimag) + x->real * y->imag OP2 x->imag * y->real);
}
sl_enddef

sl_def(FUNCTION, void,
       sl_shfparm(FLOAT, rreal),
       sl_shfparm(FLOAT, rimag),
       sl_glparm(INT, n),
       sl_glparm(COMPLEX*, sx),
       sl_glparm(INT, incx),
       sl_glparm(COMPLEX*, sy),
       sl_glparm(INT, incy))
{
  INT ix = (sl_getp(incx) < 0) ? ((-sl_getp(n) + 1) * sl_getp(incx)) : 0;
  INT iy = (sl_getp(incy) < 0) ? ((-sl_getp(n) + 1) * sl_getp(incy)) : 0;

  sl_create(,, 0, sl_getp(n),,,,
	    FUNCTION[[]]_mt,
	    sl_glarg(COMPLEX*, , sl_getp(sx) + ix),
	    sl_glarg(COMPLEX*, , sl_getp(sy) + iy),
	    sl_glarg(INT, , sl_getp(incx)),
	    sl_glarg(INT, , sl_getp(incy)),
	    sl_shfarg(FLOAT, sreal, 0),
	    sl_shfarg(FLOAT, simag, 0));
  sl_sync();
  sl_setp(rreal, sl_geta(sreal));
  sl_setp(rimag, sl_geta(simag));
}
sl_enddef
