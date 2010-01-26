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

#include <cstdlib.h>

void* malloc(size_t sz)
{
  return fast_malloc(sz);
}

void free(void* ptr)
{
  return fast_free(ptr);
}

void* calloc(size_t cnt, size_t sz)
{
  return fast_calloc(cnt, sz);
}

void* realloc(void *ptr, size_t sz)
{
  return fast_realloc(ptr, sz);
}
