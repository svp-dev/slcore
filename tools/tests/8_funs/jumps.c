//
// jumps.c: this file is part of the SL toolchain.
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

#include <svp/compiler.h>
#include <svp/testoutput.h>

int n;

static noinline
int intfoo(void) { n = n + 1; return n; }

int (*extfoo)(void) = &intfoo;

sl_def(extcall, void)
{
  n += extfoo();
}
sl_enddef

sl_def(exttailcall, void)
{
  extfoo();
}
sl_enddef

sl_def(intcall, void)
{
  n += intfoo();
}
sl_enddef

sl_def(inttailcall, void)
{
  intfoo();
}
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,,,,,, extcall); sl_sync();
  sl_create(,,,,,,, exttailcall); sl_sync();
  sl_create(,,,,,,, intcall); sl_sync();
  sl_create(,,,,,,, inttailcall); sl_sync();
  output_int(n, 1);
  output_char('\n', 1);
}
sl_enddef
