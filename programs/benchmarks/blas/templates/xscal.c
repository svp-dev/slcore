//
// xscal.c: this file is part of the SL program suite.
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

/*
// The following is the "straightforward" implementation
// and just works.

sl_def(FUNCTION[[]]_mt, void,
       sl_glfparm(FLOAT, a),
       sl_glparm(FLOAT*restrict, sx))
{
  sl_index(i);
  FLOAT *restrict lsx = sl_getp(sx) + i;
  *lsx = (*lsx) * sl_getp(a);
}
sl_enddef

sl_def(FUNCTION, void,
       sl_glparm(INT, n),
       sl_glfparm(FLOAT, a),
       sl_glparm(FLOAT*, sx),
       sl_glparm(INT, incx))
{
  INT start = (sl_getp(incx) < 0) ? ((-sl_getp(n) + 1) * sl_getp(incx)) : 0;
  INT limit = (sl_getp(incx) < 0) ? -1 : sl_getp(n);

  sl_create(,, start, limit, sl_getp(incx),,,
	    FUNCTION[[]]_mt,
	    sl_glfarg(FLOAT, , sl_getp(a)),
	    sl_glarg(FLOAT*, , sl_getp(sx)));
  sl_sync();
}
sl_enddef

*/

// Here we try to be smart.

sl_def(FUNCTION[[]]_mt, void,
       sl_glfparm(FLOAT, a))
{
  sl_index(i);
  FLOAT *restrict lsx = (FLOAT*)(void*)(long)i;
  *lsx = (*lsx) * sl_getp(a);
}
sl_enddef

sl_def(FUNCTION, void,
       sl_glparm(INT, n),
       sl_glfparm(FLOAT, a),
       sl_glparm(FLOAT*, sx),
       sl_glparm(INT, incx))
{
  INT start = (sl_getp(incx) < 0) ? ((-sl_getp(n) + 1) * sl_getp(incx)) : 0;
  INT limit = (sl_getp(incx) < 0) ? -1 : sl_getp(n);

  sl_create(,, (long)(sl_getp(sx) + start), (long)(sl_getp(sx) + limit), sizeof(FLOAT)*sl_getp(incx),,,
	    FUNCTION[[]]_mt,
	    sl_glfarg(FLOAT, , sl_getp(a)));
  sl_sync();
}
sl_enddef
