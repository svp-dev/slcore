//
// mtinput.c: this file is part of the SL toolchain.
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
#include <svp/testoutput.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "malloc/mtmalloc.h"
#include "mgsim/mtconf.h"

size_t __fibre_nitems = 0;
struct fibre_base_t *__fibre_base;

void sys_fibre_init(void* rawdata, bool copy)
{
    struct fibre_base_t *fibre_base = (struct fibre_base_t*)rawdata;

    size_t nitems = 0;
    for (size_t i = 0; fibre_base[i].tag != -1; ++i)
        ++nitems;

    __fibre_nitems = nitems;

    size_t sz;
    if (nitems == 0)
        sz = sizeof(fibre_base[0]);
    else
        sz = (size_t)fibre_base[nitems].data_offset;

    if (copy)
    {
        void *area = dlmemalign(64, sz);
        if (area == NULL)
        {
            output_string("Unable to allocate memory to copy the Fibre input data!\n", 2);
            abort();
        }
        memcpy(area, fibre_base, sz);
        __fibre_base = area;
    }
    else
    {
        __fibre_base = fibre_base;
    }

    if (verbose_boot) {
        output_string("* input data: fibre at 0x", 2);
        output_hex(__fibre_base, 2);
        output_string(", ", 2);
        output_uint(sz, 2);
        output_string(" bytes.", 2);
        output_ts(2);
        output_char('\n', 2);
    }
}
