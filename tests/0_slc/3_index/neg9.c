//
// neg9.c: this file is part of the slc project.
//
// Copyright (C) 2009,2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//
//

#define test_index_params  20,10,-1
#include "common.h"
// SLT_RUN: -- -n 20 -b 0
// SLT_RUN: -- -n 20 -b 1
// SLT_RUN: -- -n 20 -b 5
// SLT_RUN: -- -n 20 -b 20
// SLT_RUN: -- -n 20 -b 100
// SLT_RUN: -- -n 200 -b 0
// SLT_RUN: -- -n 200 -b 1
// SLT_RUN: -- -n 200 -b 5
// SLT_RUN: -- -n 200 -b 20
// SLT_RUN: -- -n 200 -b 100
// XIGNORE: ptl*:*
