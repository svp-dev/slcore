//
// undocumented.h: this file is part of the SL toolchain.
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
#ifndef SLC_UNDOCUMENTED_H
# define SLC_UNDOCUMENTED_H

#include <cstdlib.h>

/* TLS allocation/free */
#ifdef __mt_freestanding__
#define __tlsp_p(Pointer)  (((long)(Pointer)) < 0)
#define safe_free(Pointer) ((void)(__tlsp_p(Pointer) ? 0 : free(Pointer)))
#define fast_safe_free(Pointer) ((void)(__tlsp_p(Pointer) ? 0 : fast_free(Pointer)))
#else
#warning safe_free() is not defined here.
#define safe_free(Pointer) /* nothing: memory leak */
#endif

#endif
