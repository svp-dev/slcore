//
// k3.c: this file is part of the SL toolchain.
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
//      LIVERMORE KERNEL 3
//        Inner Product
//---------------------------------

//---------------------------------
//         q = 0.0;
//         for ( k=0 ; k<n ; k++ )
//         {
//             q += z[k]*x[k];
// 	   }
//---------------------------------

// muTC/SL implementation courtesy of M.A.Hicks

sl_def(innerk3, void,
       sl_shfparm(double, ql),
       sl_glparm(double*restrict, zl),
       sl_glparm(double*restrict, xl))
{
  sl_index(i);

  sl_setp(ql, sl_getp(ql) + (sl_getp(zl)[i] * sl_getp(xl)[i]));

}
sl_enddef

// LL_USE: Z X Q
sl_def(kernel3, void)
{
  //create the kernel, not forgetting to initialise
  //q to zero
  sl_create(,, 0, inner[KERNEL], 1, SHARED_BLOCK,, innerk3,
	    sl_shfarg(double, qql, 0.0),
	    sl_glarg(double*restrict, zzl, Z),
	    sl_glarg(double*restrict, xxl, X));
  sl_sync();

  //now get the returned shared and place it
  //into the data store
  *Q = sl_geta(qql);
}
sl_enddef
