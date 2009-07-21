//                                                             -*- C++ -*-
//
// sl_support.h: this file is part of the slc project.
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

#ifndef SLC_SL_SUPPORT_H
# define SLC_SL_SUPPORT_H

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
typedef uTC::place sl_place_t;

#define PLACE_DEFAULT uTC::PLACE_GROUP
#define PLACE_LOCAL   uTC::PLACE_LOCAL

#define SVP_ENOERR uTC::EXIT_NORMAL
#define SVP_EBROKEN uTC::EXIT_BREAK
#define SVP_EKILLED uTC::EXIT_KILL

#endif // ! SLC_SL_SUPPORT_H
