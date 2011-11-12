//
// stdio.h: this file is part of the SL toolchain.
//
// Copyright (C) 2009,2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#ifndef SLC_MTA_STDIO_H
# define SLC_MTA_STDIO_H

// for size_t, NULL
#include <stddef.h>
// for va_list
#include <stdarg.h>

// 7.19.1
typedef size_t fpos_t;

struct __sl_FILE;
typedef struct __sl_FILE FILE;

#ifdef EOF
#undef EOF
#endif
#define EOF (-1)

#define BUFSIZ 1024
#define FILENAME_MAX 1024

// missing: FOPEN_MAX, L_tmpnam

// missing: SEEK_CUR, SEEK_END, SEEK_SET
// missing: TMP_MAX

// missing: stdin
extern FILE* stdout;
extern FILE* stderr;
// SVP extension:
extern FILE* stddebug;

// 7.19.4

// missing: remove, rename, tmpfile, tmpnam

// 7.19.5

// missing: fclose, fflush, fopen, freopen
// missing: setbuf, setvbuf

// 7.19.6

#if defined(__GNUC__) && !defined(__AVOID_GNUISMS)
#define LIKE_PRINTF(N, M) __attribute__ ((__format__ (__printf__, N, M)));
#else
#define LIKE_PRINTF(N, M) /* nothing */
#endif

int fprintf(FILE*restrict, const char*restrict, ...) LIKE_PRINTF(2, 3);
int printf(const char*restrict, ...) LIKE_PRINTF(1, 2);

// missing: fscanf, scanf

int snprintf(char*restrict, size_t, const char*restrict, ...) LIKE_PRINTF(3, 4);
int sprintf(char*restrict, const char*restrict, ...) LIKE_PRINTF(2, 3);

// missing: sscanf, vfscanf, vscanf, vsscanf

#ifdef __va_list_available
int vfprintf(FILE*restrict, const char*restrict, va_list);
int vprintf(const char*restrict, va_list);
int vsnprintf(char*restrict, size_t, const char*restrict, va_list);
int vsprintf(char*restrict, const char*restrict, va_list);
#endif

// 7.19.7

// missing: fgetc, fgets

int fputc(int, FILE*);
int fputs(const char*restrict, FILE*restrict);

// missing: getc, getchar, gets

int putc(int, FILE*);
int putchar(int);
int puts(const char*);

// missing: ungetc

// 7.19.8

// missing: fread
size_t fwrite(const void*restrict, size_t, size_t, FILE* restrict);

// 7.19.9

// missing: fgetpos, fseek, fsetpos, ftell, rewind

// 7.19.10

// missing: clearerr
// missing: feof, ferror

void perror(const char*);

// BSD extension
extern const char * const sys_errlist[];
extern const int sys_nerr;


#endif // ! SLC_MTA_STDIO_H
