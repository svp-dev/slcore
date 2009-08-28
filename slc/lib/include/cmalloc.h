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

#ifdef __mt_freestanding__

extern void* malloc(size_t);
extern void free(void*);
extern void* calloc(size_t, size_t);
extern void* realloc(void*, size_t);

extern void* dlmalloc(size_t);
extern void dlfree(void*);
extern void* dlcalloc(size_t, size_t);
extern void* dlrealloc(void*, size_t);

#if defined(__alpha__) || defined(__mtalpha__)

#include <cstdint.h>
#include <svp/callgate.h>

#define malloc_place 5

#define fast_malloc(N) ({						\
      uint64_t __margs[3] = { (uint64_t)N, (uint64_t)&dlmalloc, 1 };	\
      CALL_WITH_INFO(((uint64_t*)__margs)+3, malloc_place);		\
      (void*)__margs[2];						\
    })

#define fast_free(P) ({							\
      uint64_t __margs[3] = { (uint64_t)P, (uint64_t)&dlfree, 1 };	\
      CALL_WITH_INFO(((uint64_t*)__margs)+3, malloc_place);		\
      (void)0;								\
    })

#define fast_realloc(P, N) ({						\
      uint64_t __margs[5] = { (uint64_t)N, 0, (uint64_t)P, (uint64_t)&dlrealloc, 2 }; \
      CALL_WITH_INFO(((uint64_t*)__margs)+5, malloc_place);		\
      (void*)__margs[4];						\
    })

#define fast_calloc(N1, N2) ({						\
      uint64_t __margs[5] = { (uint64_t)N2, 0, (uint64_t)N1, (uint64_t)&dlcalloc, 2 }; \
      CALL_WITH_INFO(((uint64_t*)__margs)+5, malloc_place);		\
      (void*)__margs[4];						\
    })

#endif

#else

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif
#endif

#ifndef fast_malloc
#define fast_malloc malloc
#endif

#ifndef fast_realloc
#define fast_realloc realloc
#endif

#ifndef fast_free
#define fast_free free
#endif

#ifndef fast_calloc
#define fast_calloc calloc
#endif


#endif // ! SLC_CMALLOC_H
