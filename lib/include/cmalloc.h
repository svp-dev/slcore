//
// cmalloc.h: this file is part of the SL toolchain.
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

#ifndef SLC_CMALLOC_H
# define SLC_CMALLOC_H

#include <cstddef.h>


#define malloc_place 5

#ifdef __mt_freestanding__

extern void* malloc(size_t);
extern void free(void*);
extern void* calloc(size_t, size_t);
extern void* realloc(void*, size_t);

#else

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#endif

#endif // ! SLC_CMALLOC_H
