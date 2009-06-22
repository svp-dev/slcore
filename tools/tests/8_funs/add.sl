//
// add.sl: this file is part of the SL toolchain.
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

m4_include(svp/iomacros.slh)

int foo(int a, int b) { return a + b; }

sl_def(t_main, void)
{
  int x = sl_funcall(,long,foo,sl_farg(int, 10), sl_farg(int, 32));
  printf("%d\n", x);
}
sl_enddef
