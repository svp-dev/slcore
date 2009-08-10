//
// cstdbool.slh: this file is part of the slc project.
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
#ifndef __CSTDBOOL_H__
# define __CSTDBOOL_H__

#ifdef __mt_freestanding__

#define bool _Bool
#define true 1
#define false 0
#define __bool_true_false_are_defined 1

#else

/* bool, true, false already defined in C++ */
#ifndef __cplusplus
#include <stdbool.h>
#endif

#endif

#endif
