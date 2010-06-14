//
// assert.h: this file is part of the SL toolchain.
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

#ifndef SLC_MTA_ASSERT_H
#define SLC_MTA_ASSERT_H

#include <svp/compiler.h>

#undef assert

#ifdef NDEBUG
#define assert(e)       ((void)0)
#else

// forward decl, we avoid including stdio/stdlib
void abort(void);
int  printf(const char * restrict, ...);

#define assert(e)  \
    ((void) (likely(e) ? 0 : __assert (#e, __FILE__, __LINE__)))
#define __assert(e, file, line) \
    ((void)printf ("%s:%u: failed assertion `%s'\n", file, line, e), abort())

#endif

#endif
