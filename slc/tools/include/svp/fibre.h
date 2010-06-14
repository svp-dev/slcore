//
// fibre.h: this file is part of the SL toolchain.
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

#ifndef SLC_SVP_FIBRE_H
# define SLC_SVP_FIBRE_H

#include <stddef.h>

extern struct fibre_base_t {
       int tag;
       size_t rank;
       ptrdiff_t shape_offset;
       ptrdiff_t data_offset;
} *__fibre_base;

#define fibre_tag(N) __fibre_base[N].tag

#define fibre_rank(N) __fibre_base[N].rank

#define fibre_shape(N) ((size_t*)(void*)((char*)(void*)__fibre_base + __fibre_base[N].shape_offset))

#define fibre_data(N) (void*)((char*)(void*)__fibre_base + __fibre_base[N].data_offset)

#endif // ! SLC_SVP_FIBRE_H
