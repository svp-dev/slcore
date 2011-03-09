//
// fibre.h: this file is part of the SL toolchain.
//
// Copyright (C) 2009,2011 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#ifndef SLC_SVP_FIBRE_H
# define SLC_SVP_FIBRE_H

#include <stddef.h>

int fibre_tag(size_t pos);
size_t fibre_rank(size_t pos);
size_t* fibre_shape(size_t pos);
void* fibre_data(size_t pos);

#if !defined(__AVOID_INLINABLE_PRIMITIVES)
extern struct fibre_base_t {
       int tag;
       size_t rank;
       ptrdiff_t shape_offset;
       ptrdiff_t data_offset;
} *__fibre_base;

#define __inline_fibre_tag(N) __fibre_base[N].tag
#define fibre_tag(N) __inline_fibre_tag(N)
#define __inline_fibre_rank(N) __fibre_base[N].rank
#define fibre_rank(N) __inline_fibre_rank(N)
#define __inline_fibre_shape(N) ((size_t*)(void*)((char*)(void*)__fibre_base + __fibre_base[N].shape_offset))
#define fibre_shape(N) __inline_fibre_shape(N)
#define __inline_fibre_data(N) (void*)((char*)(void*)__fibre_base + __fibre_base[N].data_offset)
#define fibre_data(N) __inline_fibre_data(N)
#endif

#endif // ! SLC_SVP_FIBRE_H
