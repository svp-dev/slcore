//
// x_crnotstm.sl: this file is part of the slc project.
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

sl_def(foo, void) {} sl_enddef

// XFAIL: *:C (create not statement)
sl_def(t_main, void)
{
  sl_family_t f;
  if (1)
    sl_create(f,,1,10,2,3,, foo);
    sl_sync(f);
}
sl_enddef
