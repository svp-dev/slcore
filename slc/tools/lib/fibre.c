//
// fibre.c: this file is part of the SL toolchain.
//
// Copyright (C) 2011 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include <svp/fibre.h>

#undef fibre_tag
int fibre_tag(size_t pos)
{
    return __inline_fibre_tag(pos);
}

#undef fibre_rank
size_t fibre_rank(size_t pos)
{
    return __inline_fibre_rank(pos);
}

#undef fibre_shape
size_t* fibre_shape(size_t pos)
{
    return __inline_fibre_shape(pos);
}

#undef fibre_data
void* fibre_data(size_t pos)
{
    return __inline_fibre_data(pos);
}
