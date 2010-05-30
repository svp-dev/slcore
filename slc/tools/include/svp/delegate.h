//
// delegate.h: this file is part of the SL toolchain.
//
// Copyright (C) 2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#ifndef SLC_SVP_DELEGATE_H
# define SLC_SVP_DELEGATE_H

#ifdef __mt_freestanding__

typedef long sl_place_t;

#define PLACE_LOCAL   0xC /* 4 = local, 8 = suspend */
#define PLACE_DEFAULT 0x8 /* 8 = suspend */
#define PLACE_GROUP   0xA /* 2 = local, 8 = suspend */

#else

#ifdef __cplusplus

typedef uTC::place sl_place_t;

static const sl_place_t PLACE_LOCAL = uTC::PLACE_LOCAL;
static const sl_place_t PLACE_GROUP = uTC::PLACE_GROUP;
static const sl_place_t PLACE_DEFAULT = uTC::PLACE_GROUP;

#else

#warning delegation not defined for this target.
typedef int sl_place_t;
#define PLACE_LOCAL 0
#define PLACE_DEFAULT 0
#define PLACE_GROUP 0

#endif

#endif

#define SVP_EXIT_NORMAL 0
#define SVP_EXIT_BREAK  1
#define SVP_EXIT_KILL   2

#endif
