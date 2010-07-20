//
// shfloat.c: this file is part of the SL toolchain.
//
// Copyright (C) 2010 The SL project.
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

sl_def(foo, void, sl_shfparm(double, sarg))
{
  sl_index(i);
  double x = sl_getp(sarg);
  double x2 = x;
  if (i == 0) 
      sl_setp(sarg, x2);
  else 
      sl_setp(sarg, x2+1);
}
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,0,9,1,,, foo, sl_shfarg(double, s_in, 7.5));
  sl_sync();
  output_float(sl_geta(s_in), 1, 3);
  output_char('\n', 1);
}
sl_enddef
