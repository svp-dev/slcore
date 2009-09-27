//
// cfloat.h: this file is part of the SL toolchain.
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

#ifndef SLC_CFLOAT_H
# define SLC_CFLOAT_H

#ifdef __mt_freestanding__

#include <bits/float.h>

#else
#ifdef __cplusplus
#include <cfloat>
#else
#include <float.h>
#endif
#endif

#endif // ! SLC_CFLOAT_H
