//
// thdecl.ut.c: this file is part of the slc project.
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

#include <libutc.h>

ut_decl(foo, void);
ut_decl(bar, int, ut_shparm(int, x), ut_glparm(long, y));

ut_def(t_main, void) {} ut_enddef
