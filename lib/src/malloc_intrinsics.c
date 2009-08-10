//
// malloc_intrinsics.c: this file is part of the SL toolchain.
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

#include "svp_os.h"
#include "malloc_intrinsics.h"

void abort(void) {
  __abort();
}

void mg_puts(const char*p) {
  while (*p) __write1(*p++);
}

static const char digits[] = "0123456789abcdef";

void mg_putux(unsigned long x) {
  if (x < 16) __write1(digits[x]);
  else {
      unsigned long sh = 0;
      while ((x >> sh) >= 16)
	++sh;
      while (sh) {
	unsigned long d = x >> sh;
	__write1(digits[d]);
	x = x - (d << sh);
	--sh;
      }
    }
}

#ifndef HEAP_SIZE
#define HEAP_SIZE 10*1024*1024
#endif

#ifndef PAGE_BITS
#define PAGE_BITS 12
#endif

char heap[HEAP_SIZE] __attribute__ ((aligned(4096)));
char *__brk = heap;



void *sbrk(ptrdiff_t incr) {
  unsigned long n = incr;
  if (incr & (~(0UL) << PAGE_BITS))
    incr = ((incr >> PAGE_BITS) + 1) << PAGE_BITS;
  __brk += incr;
  return __brk;
}
