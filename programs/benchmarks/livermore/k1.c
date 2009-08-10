//
// k1.c: this file is part of the SL toolchain.
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
//      LIVERMORE KERNEL 1
//        Hydro Fragment
//---------------------------------

//---------------------------------
// 'Original' C:
//        for ( k=0 ; k<n ; k++ ) {
//            x[k] = q + y[k]*( r*z[k+10] + t*z[k+11] );
//        }
//---------------------------------

// muTC/SL implementation courtesy of M.A.Hicks

//Break down kernel into two families
//this one does the 'meat'
sl_def(innerk1, void,
       sl_glparm(double*restrict, xl),
       sl_glfparm(double, ql),
       sl_glparm(double*restrict, yl),
       sl_glfparm(double, rrl),
       sl_glparm(double*restrict, zl),
       sl_glfparm(double, tl))
{
  sl_index(i);

  //now the actual calculation
  sl_getp(xl)[i] =
    sl_getp(ql) +
    sl_getp(yl)[i] * ( sl_getp(rrl)*sl_getp(zl)[i+10] + sl_getp(tl)*sl_getp(zl)[i+11] );
}
sl_enddef

// LL_USE: X Q Y R Z T
sl_def(kernel1, void)
{
  // calculate array base pointers and values here
  // to avoid offset computations in each subordinate
  // thread, and pass them as global
  // parameters

  // create the family of the appropriate size
  // specified in the 'inner' array
  sl_create(,, 0, inner[KERNEL], 1, blocksize[KERNEL],, innerk1,
	    sl_glarg(double*restrict,xxl,X),
	    sl_glfarg(double,qql,*Q),
	    sl_glarg(double*restrict,yyl,Y),
	    sl_glfarg(double,rrrl,*R),
	    sl_glarg(double*restrict,zzl,Z),
	    sl_glfarg(double,ttl,*T));
  sl_sync();
}
sl_enddef
