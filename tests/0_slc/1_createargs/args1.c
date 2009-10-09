//
// args1.c: this file is part of the SL toolchain.
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

#include <svp/abort.h>

sl_def(foo, void, sl_shparm(int, a))
{
  sl_setp(a, sl_getp(a) + 1);
}
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,,,,,, foo, sl_sharg(int, a) );
  sl_seta(a, 10);
  sl_sync();
  if (sl_geta(a) != 11) svp_abort();
  {
    sl_create(,,,,,,, foo, sl_sharg(int, a)
	      /* whitespace */);
    sl_seta(a, 20);
    sl_sync();
    if (sl_geta(a) != 21) svp_abort();
  }
  if (sl_geta(a) != 11) svp_abort();
}
sl_enddef
