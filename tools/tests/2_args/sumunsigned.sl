//
// sumunsigned.sl: this file is part of the slc project.
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
// $Id: useafter.sl 2251 2009-06-22 15:04:33Z kena $
//

sl_def(sum_int, void, sl_shparm(unsigned int, sum))
{
  sl_setp(sum, sl_getp(sum) + 1);
}
sl_enddef

sl_def(t_main, void)
{
  unsigned int i = 1;
  sl_create(,,0,9,1,,, sum_int, sl_sharg(unsigned int, s_in, i));
  sl_sync();
}
sl_enddef


 
  
