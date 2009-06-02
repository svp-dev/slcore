//
// x_nested.sl: this file is part of the slc project.
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

sl_def(foo, void) {} sl_enddef

// XFAIL: *:T (nested creates)
sl_def(t_main, void)
{
  sl_family_t f1, f2;
  sl_create(f1,,,,,,, foo);
  sl_create(f2,,,,,,, foo);
  sl_sync(f2);
  sl_sync(f1);
}
sl_enddef
