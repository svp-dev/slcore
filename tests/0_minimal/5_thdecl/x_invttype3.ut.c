//
// x_invttype3.ut.c: this file is part of the slc project.
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

ut_decl(foo, int, ut_shparm(int, a), ut_glparm(int, b));

// XFAIL: C (mismatched number of parameters)
ut_decl((*foop), int, ut_shparm(int, a), ut_glparm(int, b), ut_glparm(int, c)) = &foo;
