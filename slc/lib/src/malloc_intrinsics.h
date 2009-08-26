//
// malloc_intrinsics.h: this file is part of the SL toolchain.
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

#define restrict __restrict__
typedef unsigned long ptrdiff_t;
typedef unsigned long size_t;

void mg_puts(const char*);
void mg_putux(unsigned long);
void abort(void);

void* sbrk(ptrdiff_t);

void* memset(void *, int, size_t);
void* memcpy(void * restrict, const void* restrict, size_t);
