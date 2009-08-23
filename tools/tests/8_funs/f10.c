//
// f10.c: this file is part of the SL toolchain.
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

#include <svp/testoutput.h>
#include <svp/compiler.h>

noinline int foo(int x) { return x + 1; }
noinline int bar(int x, int y) { return x + y; }

sl_def(t_main, void)
{
  int x = 42;
  x = foo(x);
  x = bar(x, x);
  output_int(x, 1); 
  output_char('\n', 1);
}
sl_enddef
