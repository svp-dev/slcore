//
// stdlib.h: this file is part of the SL toolchain.
//
// Copyright (C) 2010,2011 The SL project.
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

int atoi(const char *);
long atol(const char *);
long long atoll(const char *); 

/* missing: atof */

double strtold(const char*restrict, char **restrict);
float strtof(const char*restrict, char**restrict);

long strtol(const char *restrict, char **restrict, int);
long long strtoll(const char *restrict, char **restrict, int);
unsigned long strtoul(const char *restrict, char **restrict, int);
unsigned long long strtoull(const char *restrict, char **restrict, int);

void* malloc(size_t);
void free(void*);
void* calloc(size_t, size_t);
void* realloc(void*, size_t);

void abort(void);

/* missing: atexit */
void exit(int status);
void _Exit(int status);


char *getenv(const char *name);

/* BSD/SysV env extensions: */
int putenv(char *string);
int setenv(const char *name, const char *value, int overwrite);
int unsetenv(const char *name);

/* missing: system, bsearch, qsort */

/* missing: abs, labs, llabs */
/* missing: div, ldiv, lldiv */
/* missing: mblen, mbtowc, wctomb, mbstowcs, wcstombs */

#endif
