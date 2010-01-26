//
// mtinit.c: this file is part of the SL toolchain.
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

#include <svp/sep.h>
#include <svp/fibre.h>
#include <svp/slr.h>
#include <cstdlib.h>
#include <cstring.h>

/* some global variables used by programs */
struct fibre_base_t *__fibre_base;
struct __slr_base_t *__slr_base;

char **environ = 0;

static
void environ_init(char *initenv)
{
    /* 
       the environment is provided as concatenated C strings separated
       by an ASCII NUL (0). The last string is followed by a
       a couple of standalone NUL bytes.
    */

    /* first: count. */
    char *p = initenv;
    size_t nvars = 0;
    if (p)
        while (p[0] != 0)
        {
            ++nvars;
            p += strlen(p) + 1;
        }

    /* then: make array of pointers to each variable */

    // (using dlmalloc directly since we are still single-threaded at
    // this point, no exclusion required)

    environ = dlmalloc((nvars + 1) * sizeof(char*));
    size_t i;
    for (i = 0, p = initenv; i < nvars; ++i, p += strlen(p) + 1)
        environ[i] = p;
    environ[i] = 0;
}

void _lib_init_routine(void* slrbase_init, void* fibrebase_init, char *initenv, void *placeconf)
{
  __slr_base = (struct __slr_base_t*) slrbase_init;
  __fibre_base = (struct fibre_base_t*) fibrebase_init;
  environ_init(initenv);
  sep_init(placeconf);
}
