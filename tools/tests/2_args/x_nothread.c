//
// x_nothread.sl: this file is part of the slc project.
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

void foo(int a, int b);

// XFAIL: *:C (not thread type)
sl_def(t_main, void)
{
  sl_create(f,,1,10,1,1,, foo, sl_glarg(int, a), sl_glarg(int, b));
  sl_sync(f);
}
sl_enddef
