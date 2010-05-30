//
// sl_support.h: this file is part of the SL toolchain.
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

#ifndef SLC_SL_SUPPORT_H
# define SLC_SL_SUPPORT_H

[[#]]ifndef SL_SUPPORT_DEFINED
[[#]]define SL_SUPPORT_DEFINED

// the following two are required to enabled
// C99 macro definitions in C++ mode
# ifndef __STDC_LIMIT_MACROS
#  define __STDC_LIMIT_MACROS 1
# endif
# ifndef __STDC_CONSTANT_MACROS
#  define __STDC_CONSTANT_MACROS 1
# endif


#ifdef USE_EXTERNAL_PTL
#include <uTC.h>
#else
#include "ptl_svp.h"
#endif

typedef uTC::family sl_family_t;

#ifndef restrict
#define restrict __restrict__
#endif

[[#]]endif

#endif // ! SLC_SL_SUPPORT_H
