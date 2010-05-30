//
// bigarray2.c: this file is part of the SL toolchain.
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

#include <svp/compiler.h>
#include <svp/testoutput.h>

noinline
int foo(int*a) { barrier(); return a[42]; }

// XIGNORE: ptl*:R

sl_def(bar, void)
{
  int a[90000];
  a[42] = 123;
  output_int(foo(a), 1);
  output_char('\n', 1);
  output_int(a[42], 1);
  output_char('\n', 1);
}
sl_enddef

sl_def(t_main, void)
{
  sl_proccall(bar);
}
sl_enddef
