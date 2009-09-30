//
// fft_test1.c: this file is part of the SL toolchain.
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

[[]]

// SLT_RUN:  M=3 -dPi  Pf=1 -dPc  BR=1
// SLT_RUN:  M=4 -dPi  Pf=1 -dPc  BR=1

// FIXME: as of 2004-04-05 the output of this
// program seems implementation dependent;
// so ignore the diff during testing:
// XIGNORE: *:D


#include "fft_test.c"
#include "fft_impl1.c"
#include "fft_extra.c"
