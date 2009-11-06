//
// simple_synch.c: this file is part of the SL toolchain.
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

#include <svp/testoutput.h>

sl_def(foo, void, sl_shparm(int, sarg))
{
  int temp; 
  temp = sl_getp(sarg);
  sl_setp(sarg, temp);
}
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,0,9,1,,, foo, sl_sharg(int, s_in, 7));
  sl_sync();
  output_int(sl_geta(s_in), 1);
}
sl_enddef
