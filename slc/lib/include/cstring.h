//
// cstring.h: this file is part of the SL toolchain.
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

#ifndef SLC_CSTRING_H
# define SLC_CSTRING_H

#include <cstddef.h>

#ifdef __mt_freestanding__

extern void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
extern void *memmove(void *s1, const void *s2, size_t n);

extern char *strcpy(char * restrict dst, const char * restrict src);
extern char *strncpy(char * restrict dst, const char * restrict src, size_t len);

/* missing:
   strcat
   strncat
   memcmp
   strcmp
   strcoll
*/

extern int strncmp(const char *s1, const char *s2, size_t n);

/* missing:
   strxfrm
   memchr
*/

extern char *strchr(const char *s, int c);

/* missing:
   strcspn
   strpbrk
   strrchr
   strspn
   strstr
   strtok
*/
extern void *memset(void *b, int c, size_t len);

/* missing:
   strerror
*/

extern size_t strlen(const char*);

/* POSIX extensions: */
extern char *stpcpy(char * restrict dst, const char * restrict src);
extern char *stpncpy(char * restrict dst, const char * restrict src, size_t len);

/* BSD extension: */
extern char *strdup(const char *);

#ifndef shutup_cstring_h
#warning this implementation of string.h is incomplete.
#endif

#else

#ifdef __cplusplus
#include <cstring>
#else
#include <string.h>
#endif

#endif

#endif
