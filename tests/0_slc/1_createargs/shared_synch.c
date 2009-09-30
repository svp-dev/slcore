//
// shared_synch.c
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


sl_def(synch_thread, void, sl_shparm(int, s))
{
  int temp;

  temp = sl_getp(s);
  output_int(temp, 1);
  sl_setp(s, temp + 1);
}
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,0,9,1,,, synch_thread, sl_sharg(int, s_in, 0));
  sl_sync();
}
sl_enddef
