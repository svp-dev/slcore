//
// k11.c: this file is part of the SL toolchain.
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

[[]]
//---------------------------------
//      LIVERMORE KERNEL 11
//        First Sum
//---------------------------------

//---------------------------------
// x[0] = y[0];
// for ( k=1 ; k<n ; k++ )
// {
//   x[k] = x[k-1] + y[k];
// }
//---------------------------------

// muTC/SL implementation courtesy of M.A.Hicks

sl_def(innerk11, void,
       sl_glparm(double*restrict, xl),
       sl_glparm(double*restrict, yl),
       sl_shfparm(double, lastit))
{
  sl_index(i);
  double temp = sl_getp(lastit) + sl_getp(yl)[i];
  sl_getp(xl)[i] = temp;
  sl_setp(lastit, temp);
}
sl_enddef

// LL_USE: X Y
sl_def(kernel11,void)
{
  sl_create(,, 1, inner[KERNEL], 1, SHARED_BLOCK,, innerk11,
	    sl_glarg(double*restrict, xxl, X),
	    sl_glarg(double*restrict, yyl, Y),
	    sl_shfarg(double, llastit, Y[0]));
  sl_sync();
}
sl_enddef
