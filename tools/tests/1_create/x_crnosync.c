//
// x_crnosync.sl: this file is part of the slc project.
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

#include <svp/compiler.h>

sl_def(func, void)
{
  nop();
}
sl_enddef

/*
 * create without sync should fail during transform
 * XFAIL: *:T 
 */

sl_def(t_main, void)
{
  sl_family_t f;
  sl_create(f,,0,1,1,,, func);
}
sl_enddef
