//
// string.h: this file is part of the SL toolchain.
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

#ifndef SLC_MTA_STRING_H
# define SLC_MTA_STRING_H

#include <stddef.h>

void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
void *memmove(void *s1, const void *s2, size_t n);

char *strcpy(char * restrict dst, const char * restrict src);
char *strncpy(char * restrict dst, const char * restrict src, size_t len);


char *strcat(char *restrict s1, const char *restrict s2);
char *strncat(char *restrict s1, const char *restrict s2, size_t n);

/* missing:
   memcmp
   strcoll
*/

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

/* missing:
   strxfrm
   memchr
*/

char *strchr(const char *s, int c);

/* missing:
   strcspn
   strpbrk
   strrchr
   strspn
   strstr
   strtok
*/
void *memset(void *b, int c, size_t len);

char *strerror(int errnum);

size_t strlen(const char*);

/* POSIX extensions: */
char *stpcpy(char * restrict dst, const char * restrict src);
char *stpncpy(char * restrict dst, const char * restrict src, size_t len);
int strerror_r(int errnum, char *strerrbuf, size_t buflen);

/* BSD extensions: */
char *strdup(const char *);
size_t strnlen(const char*, size_t);
size_t strlcpy(char * restrict dst, const char * restrict src, size_t len);
size_t strlcat(char * restrict dst, const char * restrict src, size_t len);

#endif
