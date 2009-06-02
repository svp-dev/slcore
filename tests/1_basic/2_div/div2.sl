//
// div2.sl: this file is part of the slc project.
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
m4_include(svp/assert.slh)
m4_include(svp/div.slh)

sl_def(t_main, void)
{
  unsigned long x = 69, y = 5;
  divmodu(x, y);
  unsigned long x1 = x, y1 = y;
  while (x1--) putc('.'); putc('\n');
  while (y1--) putc('.'); putc('\n');
  svp_assert(x == 4);
  svp_assert(y == 13);
}
sl_enddef
