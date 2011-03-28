//
// delegate.h: this file is part of the SL toolchain.
//
// Copyright (C) 2010,2011 The SL project.
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


/* sl_place_t: data type for place identifiers */

#if defined(__slc_os_host_ptl__)

typedef uTC::place sl_place_t;

#else

typedef unsigned long sl_place_t;

#endif

/* family exit codes */

#define SVP_EXIT_NORMAL 0
#define SVP_EXIT_BREAK  1
#define SVP_EXIT_KILL   2

/* introspection */

extern sl_place_t get_current_place(void);
extern unsigned long get_core_id(void);
extern sl_place_t get_local_place(void);

#if !defined(__AVOID_INLINABLE_PRIMITIVES) && defined(__mt_freestanding__) && \
    defined(__GNUC__) && !defined(__AVOID_GNUISMS)

#include <svp/compiler.h>

alwaysinline
sl_place_t __inline_get_current_place(void)
{
    sl_place_t p;
    __asm__("getpid %0" : "=r"(p));
    return p;
}
#define get_current_place() __inline_get_current_place()

alwaysinline
unsigned long __inline_get_core_id(void)
{
    unsigned long p;
    __asm__("getcid %0" : "=r"(p));
    return p;
}
#define get_core_id() __inline_get_core_id()

alwaysinline
sl_place_t __inline_get_local_place(void)
{
    unsigned long cid = get_core_id();
    return (cid << 1) | 1;
}
#define get_local_place() __inline_get_local_place()

#endif

#if defined(__slc_os_sim__)

#define PLACE_DEFAULT 0
#define PLACE_LOCAL get_local_place()

#else

#if defined(__slc_os_host_ptl__)

static const sl_place_t PLACE_DEFAULT = uTC::PLACE_GROUP;
static const sl_place_t PLACE_LOCAL = uTC::PLACE_LOCAL;

#else

#define PLACE_DEFAULT 0
#define PLACE_LOCAL   0

#endif

#endif

#endif
