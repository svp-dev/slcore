//
// checkers.c: this file is part of the SL toolchain.
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

#include <svp/gfx.h>

sl_def(t_main, void)
{
  gfx_init();
  gfx_resize(8, 8);

  unsigned long x;
  for (x = 0; x < 64; ++x) 
    gfx_fb_set(x, ((x + x/2)%2) ? 0xffffff : 0);

  gfx_dump(0, 1, 0, 0);
  gfx_close();
}
sl_enddef
