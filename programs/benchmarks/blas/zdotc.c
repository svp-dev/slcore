//
// zdotc.sl: this file is part of the slc project.
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

#define COMPLEX blas_zomplex
#define FLOAT double
#define FUNCTION zdotc
#define OP1 +
#define OP2 -
#include "templates/xdotc.c"
