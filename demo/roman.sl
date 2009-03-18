//                                                             -*- m4 -*-
// roman.sl: this file is part of the slc project.
//
// Copyright (C) 2009 The SL project.
// All rights reserved.
//
// $Id$
//
m4_include(svp/roman.slh)

sl_def(t_main, void)
{
  roman(42);
  putc('\n');
}
sl_enddef
