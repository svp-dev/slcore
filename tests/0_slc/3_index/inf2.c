//
// inf2.c: this file is part of the slc project.
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

#define test_index_params  0,-1,0
#include "common.h"
// SLT_RUN: -dN=20 -dB=0
// SLT_RUN: -dN=20 -dB=1
// SLT_RUN: -dN=20 -dB=5
// SLT_RUN: -dN=20 -dB=20
// SLT_RUN: -dN=20 -dB=100
// SLT_RUN: -dN=200 -dB=0
// SLT_RUN: -dN=200 -dB=1
// SLT_RUN: -dN=200 -dB=5
// SLT_RUN: -dN=200 -dB=20
// SLT_RUN: -dN=200 -dB=100
// XIGNORE: ptl*:*
