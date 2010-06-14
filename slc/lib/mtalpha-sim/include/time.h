//
// time.h: this file is part of the SL toolchain.
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
#ifndef SLC_MTA_TIME_H
# define SLC_MTA_TIME_H

#include <sys/types.h>
#include <stddef.h>

#define CLOCKS_PER_SEC 1000000000

extern clock_t clock(void);

#if defined(__alpha__) || defined(__mtalpha__)
__attribute__((__always_inline__))
static clock_t __inline_clock(void)
{
    clock_t c;                                               
    __asm__ __volatile__ ("rpcc %0" : "=r"(c) : : "memory");
    return c;
}
#else
#warning Reading the time stamp counter is not defined on your system.
#define __inline_clock() ((clock_t)-1)
#endif

#define clock() __inline_clock()

#ifndef shutup_ctime_h
#warning this implementation of time.h is incomplete.
#endif

#endif // ! SLC_MTA_TIME_H
