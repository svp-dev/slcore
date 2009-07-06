//
// sah-quicksort.sl: this file is part of the slc project.
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

m4_include(cstddef.slh)
m4_include(svp/perf.slh)
m4_include(svp/iomacros.slh)
m4_include(slr.slh)

m4_define(DLEN, 1024)

slr_decl(slr_var(int, Pc, "if nonempty: print number of cycles used"));

sl_def(computeDataT, void, sl_glparm(int*, data), sl_glparm(int, length))
{
  sl_index(cnt);

  sl_getp(data)[cnt] = (cnt % 2) ? cnt : sl_getp(length) - cnt;
}
sl_enddef

sl_def(computeData, void, sl_glparm(int*, data), sl_glparm(int, length))
{
  sl_create(,,,sl_getp(length),,,,computeDataT,
    sl_glarg(int*, dummy1, sl_getp(data)),
    sl_glarg(int, dummy2, sl_getp(length)));
  sl_sync();
}
sl_enddef

sl_def(printVectorT, void, sl_glparm(int*, data), sl_shparm(int, barrier))
{
  sl_index(i);
  int val = sl_getp(data)[i];
  int baton = sl_getp(barrier);

  putn(val);
  putc('\n');

  sl_setp(barrier, baton);
}
sl_enddef

sl_def(printVector, void, sl_glparm(int*, data), sl_glparm(int, length))
{
  int barrier;

  puts("Dim: 1\nShape: [");
  putn( sl_getp(length));
  puts("]\nData: ");

  sl_create(,,,sl_getp(length),,2,,printVectorT,
    sl_glarg(int*, dummy1, sl_getp(data)),
    sl_sharg(int, barrier, 0));
  sl_sync();

  puts("\n");
}
sl_enddef

sl_def(t_main, void)
{
  int64_t tStart, tEnd;
  int data[DLEN];
  int length = DLEN;

  sl_proccall(computeData, sl_glarg(int*, dummy1, data), sl_glarg(int, dummy2, length));

  sl_proccall(printVector, sl_glarg(int*, dummy1, data), sl_glarg(int, dummy2, length));

}
sl_enddef
