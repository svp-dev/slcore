//
// cstrings.h: this file is part of the SL toolchain.
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

#ifndef SLC_CSTRINGS_H
# define SLC_CSTRINGS_H

#include <cstddef.h>

#ifdef __mt_freestanding__

void     bcopy(const void *, void *, size_t);                   /* LEGACY */
void     bzero(void *, size_t);                                 /* LEGACY */

int      ffs(int);
#define  ffs(X) __builtin_ffs(X)

#ifndef shutup_cstrings_h
#warning this implementation of strings.h is incomplete.
#endif

#else

#include <strings.h>

#endif

#endif
