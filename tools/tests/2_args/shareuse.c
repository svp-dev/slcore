//
// shareuse.c: this file is part of the SL toolchain.
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

sl_def(use_share, void, sl_shparm(int, s))
{
  int z;
  z = sl_getp(s);  /* read first, to "consume" from previous thread */
  sl_setp(s, 10);  /* write constant to next thread */
}
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,0,9,1,,, use_share, sl_sharg(int, s_in, 0));
  sl_sync();
}
sl_enddef
