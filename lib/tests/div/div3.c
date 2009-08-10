//
// div3.c: this file is part of the slc project.
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

#include <svp/div.h>

sl_def(t_main, void)
{
  unsigned long xqu = 42, yqu = 5;
  divmodu(xqu, yqu);

  signed long xqs = 42, yqs = 5;
  divmods(xqs, yqs);
}
sl_enddef
