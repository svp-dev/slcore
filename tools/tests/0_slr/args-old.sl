//
// args.sl: this file is part of the slc project.
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

m4_include(slr.slh)
m4_include(svp/iomacros.slh)

slr_decl(slr_var(int, a), slr_var(int, b));

// SLT_RUN:  a=10  b=-10

sl_def(t_main, void)
{
  int a = slr_get(a)[0];
  int b = slr_get(b)[0];
  printf("%d + %d = %d\n", a, b, a+b);
}
sl_enddef
