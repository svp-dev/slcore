//
// m-problem.sl: this file is part of the slc project.
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

// PROBLEM: the following program
// prints "42", although SVP suggests
// it should print "10" instead.

int blah;

sl_def(foo, void)
{
  blah = 10;
}
sl_enddef

sl_def(t_main, void)
{
  blah = 42;
  sl_proccall(foo);
  putn(blah); putc('\n');
}
sl_enddef
