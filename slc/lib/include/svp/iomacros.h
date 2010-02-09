//
// iomacros.h: this file is part of the SL toolchain.
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

#ifndef SLC_SVP_IOMACROS_H
# define SLC_SVP_IOMACROS_H

#include <cstdio.h>
#include <cstdint.h>

#warning this header is deprecated. Use cstdio.h instead.

#define putf(N, Prec) printf("%.*lf", (int)(Prec), (double)(N))

#define putn(N) printf("%jd", (intmax_t)(N))

#define putu(N) printf("%ju", (uintmax_t)(N))

#endif // ! SLC_SVP_IOMACROS_H
