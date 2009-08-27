//
// brk_floats.ut.c: this file is part of the SL toolchain.
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

#include <libutc.h>

//test break with some control flow

ut_def(foo, float)
{

  float  x = 10.78;

  ut_break(x);

}
ut_enddef


ut_def(foo2, double)
{

  double y = 15.294;

  ut_break(y);

}
ut_enddef


ut_def(t_main, void)
{
  // do nothing
}
ut_enddef
