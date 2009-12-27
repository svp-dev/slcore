//
// kernel11.c: this file is part of the SL program suite.
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
// Livemore Loops -- SLC (uTC)
// M.A.Hicks, CSA Group, UvA
// Implementation based on various
// reference implementations
// including the original FORTRAN
// but mostly from
// Roy Longbottom, 1996.
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

sl_def(innerk11,void,
       sl_glparm(double*restrict, X),
       sl_glparm(const double*restrict, Y),
       sl_shfparm(double, Xkm1))
{
    sl_index(k);
    double temp;
    sl_setp(Xkm1, temp = sl_getp(Y)[k] + sl_getp(Xkm1));
    sl_getp(X)[k] = temp;
}
sl_enddef

sl_def(kernel11,void,
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n)
       , sl_glparm(double*restrict, X)
       , sl_glparm(size_t, X_dim0)
       , sl_glparm(const double*restrict, Y)
       , sl_glparm(size_t, Y_dim0)
    )
{
    sl_getp(X)[0] = sl_getp(Y)[0];
    sl_create(,, 1, sl_getp(n),,,,innerk11,
              sl_glarg(double*, , sl_getp(X)),
              sl_glarg(const double*, , sl_getp(Y)),
              sl_shfarg(double, Xkm1, sl_getp(Y[0])));
    sl_sync();
}
sl_enddef
