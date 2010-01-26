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

#include <cstddef.h>
#include <cmalloc.h>

/* C string library */
#ifdef __mt_freestanding__
extern size_t strlen(const char*);
extern char *stpcpy(char * restrict dst, const char * restrict src);
extern char *stpncpy(char * restrict dst, const char * restrict src, size_t len);
extern char *strcpy(char * restrict dst, const char * restrict src);
extern char* strncpy(char * restrict dst, const char * restrict src, size_t len);
extern int strncmp(const char *s1, const char *s2, size_t n);
extern char *strdup(const char *s1);
extern char *strchr(const char *s, int c);

extern void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
extern void *memmove(void *s1, const void *s2, size_t n);
extern void *memset(void *b, int c, size_t len);
#else
#ifdef __cplusplus
#include <cstring>
#else
#include <string.h>
#endif
#endif

/* TLS allocation/free */
#ifdef __mt_freestanding__
#define __tlsp_p(Pointer)  (((long)(Pointer)) < 0)
#define safe_free(Pointer) ((void)(__tlsp_p(Pointer) ? 0 : free(Pointer)))
#define fast_safe_free(Pointer) ((void)(__tlsp_p(Pointer) ? 0 : fast_free(Pointer)))
#else
#warning safe_free() is not defined here.
#define safe_free(Pointer) /* nothing: memory leak */
#endif

/* C stdlib */
#ifdef __mt_freestanding__
extern char **environ;
extern char *getenv(const char *name);
extern int putenv(char *string);
extern int setenv(const char *name, const char *value, int overwrite);
extern int unsetenv(const char *name);
#else
#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif
#endif

#endif
