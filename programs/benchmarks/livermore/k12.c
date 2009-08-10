//
// k12.c: this file is part of the SL toolchain.
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
//      LIVERMORE KERNEL 12
//       First Difference
//---------------------------------

//---------------------------------
// for ( k=0 ; k<n ; k++ )
// {
//   x[k] = y[k+1] - y[k];
// }
//---------------------------------

// muTC/SL implementation courtesy of M.A.Hicks

sl_def(innerk12, void,
       sl_glparm(double*restrict, xl),
       sl_glparm(double*restrict, yl))
{
  sl_index(i);
  sl_getp(xl)[i] = sl_getp(yl)[i+1] - sl_getp(yl)[i];
}
sl_enddef

// LL_USE: X Y
sl_def(kernel12,void)
{
  sl_create(,, 0, inner[KERNEL],1, blocksize[KERNEL],, innerk12,
	    sl_glarg(double*restrict, xxl, X),
	    sl_glarg(double*restrict, yyl, Y));
  sl_sync();
}
sl_enddef
