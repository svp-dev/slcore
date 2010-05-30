//
// useexit2.c: this file is part of the SL toolchain.
//
// Copyright (C) 2009,2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include <svp/compiler.h>
#include <svp/delegate.h>

// XIGNORE: *:*

sl_def(foo, void) { sl_break; } sl_enddef

sl_def(t_main, void)
{
  int r;
  sl_create(,,,,,, void, foo);
  sl_sync(r);
  if (r == SVP_EXIT_BREAK)
    nop();
}
sl_enddef
