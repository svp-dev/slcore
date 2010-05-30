//
// crdefaults.c: this file is part of the SL toolchain.
//
// Copyright (C) 2009,2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

int bla;

sl_def(a, void, sl_shparm(int, x), sl_shparm(int, v))
{
  bla = sl_getp(x);
  sl_setp(v, sl_getp(v) + bla);
}
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,,,,,, a, sl_sharg(int, x, 0), sl_sharg(int, v, 0));
  sl_sync();
}
sl_enddef
