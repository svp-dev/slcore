//
// sac_helpers.h: this file is part of the SL toolchain.
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

#ifndef SLC_SAC_HELPERS_H
# define SLC_SAC_HELPERS_H

#include <cstddef.h>
#include <cmalloc.h>
#include <calloca.h>

#ifdef __mt_freestanding__

extern size_t strlen(const char*);
extern char* strncpy(char *, const char*, size_t);

#define __tlsp_p(Pointer)  (((long)(Pointer)) < 0)
#define safe_free(Pointer) ((void)(__tlsp_p(Pointer) ? 0 : free(Pointer)))
#define fast_safe_free(Pointer) ((void)(__tlsp_p(Pointer) ? 0 : fast_free(Pointer)))

#else

#include <string.h>

#warning safe_free() is not defined here.

#endif

#endif // ! SLC_SAC_HELPERS_H
