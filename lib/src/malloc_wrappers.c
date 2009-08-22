//
// malloc_wrappers.c: this file is part of the SL toolchain.
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

#include <cmalloc.h>
#include "callgate.h"

#define malloc_place 5
extern void* dlmalloc(size_t);
extern void dlfree(void*);
extern void* dlcalloc(size_t, size_t);
extern void* dlrealloc(void*, size_t);

void* malloc(size_t sz)
{
  struct callinfo m;
  m.head0.u = 1;
  m.head1.fptr = dlmalloc;
  m.arg0.u = sz;
  CALL_WITH_INFO(m, malloc_place);
  return m.head0.p;
}

void free(void* ptr)
{
  struct callinfo m;
  m.head0.u = 1;
  m.head1.fptr = dlfree;
  m.arg0.p = ptr;
  CALL_WITH_INFO(m, malloc_place);
}

void* calloc(size_t cnt, size_t sz)
{
  struct callinfo m;
  m.head0.u = 2;
  m.head1.fptr = dlcalloc;
  m.arg0.u = cnt;
  m.arg1.u = sz;
  CALL_WITH_INFO(m, malloc_place);
  return m.head0.p;
}

void* realloc(void *ptr, size_t sz)
{
  struct callinfo m;
  m.head0.u = 2;
  m.head1.fptr = dlrealloc;
  m.arg0.p = ptr;
  m.arg1.u = sz;
  CALL_WITH_INFO(m, malloc_place);
  return m.head0.p;
}
