//
// stdlib.h: this file is part of the SL toolchain.
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

#ifndef SLC_MTA_STDLIB_H
# define SLC_MTA_STDLIB_H

#include <stddef.h>


/* missing: div_t, ldiv_t, lldiv_t */

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

/* missing: RAND_MAX, rand, srand */

/* missing: MB_CUR_MAX, */

extern int atoi(const char *);
extern long atol(const char *);
extern long long atoll(const char *); 

/* missing: atof, strtold */

double strtold(const char*restrict, char **restrict);
float strtof(const char*restrict, char**restrict);

extern long strtol(const char *restrict, char **restrict, int);
extern long long strtoll(const char *restrict, char **restrict, int);
extern unsigned long strtoul(const char *restrict, char **restrict, int);
extern unsigned long long strtoull(const char *restrict, char **restrict, int);

extern void* malloc(size_t);
extern void free(void*);
extern void* calloc(size_t, size_t);
extern void* realloc(void*, size_t);

/* low-level, thread-unsafe implementations */
extern void* dlmalloc(size_t);
extern void dlfree(void*);
extern void* dlcalloc(size_t, size_t);
extern void* dlrealloc(void*, size_t);

/* "fast" aliases */
#if defined(__alpha__) || defined(__mtalpha__)

#include <stdint.h>
#include <svp/callgate.h>

#define malloc_place 0xf /* 8=suspend, 4|2=delegate, 1=exclusive (implicit core ID = 0) */

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

void abort(void);

/* missing: atexit, exit, _Exit */

extern char *getenv(const char *name);

/* BSD/SysV env extensions: */
extern int putenv(char *string);
extern int setenv(const char *name, const char *value, int overwrite);
extern int unsetenv(const char *name);

/* missing: system, bsearch, qsort */

/* missing: abs, labs, llabs */
/* missing: div, ldiv, lldiv */
/* missing: mblen, mbtowc, wctomb, mbstowcs, wcstombs */

#ifndef shutup_cstdlib_h
#warning this implementation of stdlib.h is incomplete.
#endif


#endif
