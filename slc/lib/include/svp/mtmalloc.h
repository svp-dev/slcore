//
// mtmalloc.h: this file is part of the SL toolchain.
//
// Copyright (C) 2011 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//
#ifndef SLC_MTA_MTMALLOC_H
#define SLC_MTA_MTMALLOC_H

#include <stddef.h>

/* low-level, thread-unsafe implementations */
void* dlmalloc(size_t);
void dlfree(void*);
void* dlcalloc(size_t, size_t);
void* dlrealloc(void*, size_t);

/* "fast" aliases */
#if defined(__mt_freestanding__) && (defined(__alpha__) || defined(__mtalpha__))

#include <stdint.h>
#include <svp/callgate.h>

#define malloc_place 0xf /* 8=suspend, 4|2=delegate, 1=exclusive (implicit core ID = 0) */

#define excl_dlmalloc(N) ({						\
      uint64_t __margs[3] = { (uint64_t)N, (uint64_t)&dlmalloc, 1 };	\
      CALL_WITH_INFO(((uint64_t*)__margs)+3, malloc_place);		\
      (void*)__margs[2];						\
    })

#define excl_dlfree(P) ({							\
      uint64_t __margs[3] = { (uint64_t)P, (uint64_t)&dlfree, 1 };	\
      CALL_WITH_INFO(((uint64_t*)__margs)+3, malloc_place);		\
      (void)0;								\
    })

#define excl_dlrealloc(P, N) ({						\
      uint64_t __margs[5] = { (uint64_t)N, 0, (uint64_t)P, (uint64_t)&dlrealloc, 2 }; \
      CALL_WITH_INFO(((uint64_t*)__margs)+5, malloc_place);		\
      (void*)__margs[4];						\
    })

#define excl_dlcalloc(N1, N2) ({						\
      uint64_t __margs[5] = { (uint64_t)N2, 0, (uint64_t)N1, (uint64_t)&dlcalloc, 2 }; \
      CALL_WITH_INFO(((uint64_t*)__margs)+5, malloc_place);		\
      (void*)__margs[4];						\
    })

#endif

#endif
