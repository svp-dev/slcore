//
// ctime.h: this file is part of the SL toolchain.
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
#ifndef SLC_CTIME_H
# define SLC_CTIME_H

#ifdef __mt_freestanding__

#include <cstddef.h>

#define CLOCKS_PER_SEC 1000000000
typedef long clock_t;

extern clock_t clock(void);

#if defined(__alpha__) || defined(__mtalpha__)
#define __inline_clock() ({ \
      clock_t __cycles_c__;					      \
      __asm__ __volatile__ ("rpcc %0" : "=r"(__cycles_c__) : : "memory"); \
      __cycles_c__;							\
    })
#else
#warning Reading the time stamp counter is not defined on your system.
#define __inline_clock() ((clock_t)-1)
#endif

#define clock() __inline_clock()

#warning this implementation of time.h is incomplete.

#else
#ifdef __cplusplus
#include <ctime>
#else
#include <time.h>
#endif
#endif

#endif // ! SLC_CTIME_H
