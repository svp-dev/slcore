//
// twoways.c: this file is part of the SL toolchain.
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

sl_def(foo, void, sl_shparm(int, x))
{
  sl_setp(x, sl_getp(x));
}
sl_enddef


sl_def(bar, void, sl_shparm(int, y))
{
  sl_create(,,,,,,,foo, sl_sharg(int, sx));

  sl_seta(sx, sl_getp(y));

  sl_sync();

  sl_setp(y, sl_geta(sx));
}
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,,,,,, bar, sl_sharg(int, x));
  sl_seta(x, 0);
  sl_sync();
}
sl_enddef
