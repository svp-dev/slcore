//
// stddef.h: this file is part of the SL toolchain.
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

#ifndef SLC_MTA_STDDEF_H
# define SLC_MTA_STDDEF_H

#if defined(__alpha__)||defined(__mtalpha__)||defined(__mtsparc__)||defined(__mips__)

/* 7.17.2 types */

typedef long ptrdiff_t;
typedef unsigned long size_t;

typedef int wchar_t;

#else
# warning No stddef.h definitions available for this target.
#endif

/* 7.17.3 macros */

#ifndef NULL
#define NULL ((void *)0)
#endif

#define offsetof(TYPE, MEMBER) __builtin_offsetof (TYPE, MEMBER)

#endif // ! SLC_MTA_STDDEF_H
