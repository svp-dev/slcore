//
// globpass.sl: this file is part of the slc project.
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

m4_include(svp/lib.slh)

sl_def(print_glob, void, sl_glparm(int, num))
{
  if (sl_getp(num) > 10) {
    svp_nop();
  }
}
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,0,9,1,,, print_glob, sl_glarg(int, g, 5));
  sl_sync();
}
sl_enddef


 
  
