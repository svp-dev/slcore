//
// mtinit.c: this file is part of the SL toolchain.
//
// Copyright (C) 2009,2010,2011 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//
#include <svp/compiler.h>
#include <svp/testoutput.h>
#include <svp/sep.h>
#include <svp/fibre.h>
#include <svp/slr.h>
#include <svp/mtmalloc.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mtconf.h"
#include "heap.h"

static noinline
void sys_environ_init(char *initenv)
{
    extern char** environ;

    /* 
       the environment is provided as concatenated C strings separated
       by an ASCII NUL (0). The last string is followed by a
       a couple of standalone NUL bytes.
    */

    // (using dlmalloc directly since we are still single-threaded at
    // this point, no exclusion required)

    /* first: count, and detect verbosity. */
    char *p = initenv;
    size_t nvars = 0;
    if (p)
        while (p[0] != 0)
        {
            ++nvars;
            size_t sz = strlen(p);
            if (strncmp("MGSYS_QUIET=", p, (sz > 12) ? 12 : sz) == 0)
                verbose_boot = 0;
            p += sz + 1;
        }

    size_t envdatasize = p - initenv;

    /* then: duplicate the data, since it may be in ROM */
    if (verbose_boot)
    {
        output_string("* input data: env at 0x", 2);
        output_hex(initenv, 2);
        output_string(", ", 2);
        output_uint(envdatasize, 2);
        output_string(" bytes.", 2);
        output_ts(2);
        output_char('\n', 2);
    }

    char *area = dlmalloc(envdatasize);
    memcpy(area, initenv, envdatasize);

    if (verbose_boot)
    {
        output_string("* strings at 0x", 2);
        output_hex(area, 2);
    }

    /* then: make array of pointers to each variable */
    environ = dlmalloc((nvars + 1) * sizeof(char*));

    size_t i;
    for (i = 0, p = area; i < nvars; ++i, p += strlen(p) + 1)
    {
        environ[i] = p;
    }
    environ[i] = 0;

    if (verbose_boot)
    {
        output_string(", environ[", 2);
        output_uint(nvars, 2);
        output_string("] at 0x", 2);
        output_hex(environ, 2);
        output_string(".", 2);
        output_ts(2);
        output_char('\n', 2);
    }
}

extern sl_place_t __main_place_id;

static noinline
void sys_set_main_pid(unsigned req_ncores, unsigned req_placesize)
{
    if (verbose_boot) output_string(" -> request to SEP...", 2);

    sl_place_t p;

    int r = sep_alloc(root_sep, &p, SAL_EXACT, req_placesize);
    if (r == -1) 
    {
        if (verbose_boot)
            output_string(" failed", 2);
        abort();
    }
    if (verbose_boot) 
    {
        p = p & (p - 1); // drop the existing size bit
        p |= req_ncores; // add the new size bit

        __main_place_id = p;

        output_string(" success, t_main goes to pid=0x", 2);
        output_hex(p, 2);
        output_ts(2);
        output_char('\n', 2);
    }
}

static noinline
void sys_check_ncores(void)
{
    char *v = getenv("SLR_NCORES");
    char *v2 = getenv("SLR_PLACESIZE");
    if (v && v[0]) {
        if (verbose_boot) {
            output_string("* -n ", 2);
            output_string(v, 2);
        }
        unsigned req_ncores = strtoul(v, 0, 10);
        if (req_ncores && ((req_ncores & (req_ncores - 1)) == 0))
        {
            unsigned req_placesize = 0;
            if (v2 && v2[0])
                req_placesize = strtoul(v2, 0, 10);
            req_placesize = req_placesize ? req_placesize : req_ncores;
            if (req_ncores > req_placesize)
                req_ncores = req_placesize;

            sys_set_main_pid(req_ncores, req_placesize);
        }
        else
            if (verbose_boot) 
                output_string(" (invalid, ignoring)\n", 2);
    }
}

sl_place_t __stdio_place_id;
sl_place_t __malloc_place_id;
sl_place_t __dtoa_place_id;

static noinline
void sys_places_init(void)
{
    if (verbose_boot) output_string("* allocating places for the C stdlib...", 2);

    int r1 = sep_alloc(root_sep, &__stdio_place_id, SAL_MIN, 1);
    int r2 = sep_alloc(root_sep, &__malloc_place_id, SAL_MIN, 1);
    int r3 = sep_alloc(root_sep, &__dtoa_place_id, SAL_MIN, 1);
    if (r1 == -1 || r2 == -1 || r3 == -1)
    {
        if (verbose_boot) output_string(" failed", 2);
        abort();
    }

    if (verbose_boot)
    {
        output_string(" success: stdio at 0x", 2);
        output_hex(__stdio_place_id, 2);
        output_string(", malloc at 0x", 2);
        output_hex(__malloc_place_id, 2);
        output_string(", dtoa at 0x", 2);
        output_hex(__dtoa_place_id, 2);
        output_ts(2);
        output_char('\n', 2);        
    }
}


extern void sys_sep_init(void);
extern void sys_fibre_init(void*, bool);
extern void sys_vars_init(void*, bool);
extern void sys_argv_init(void);

void sys_init(void* slrbase_init, 
              void* fibrebase_init, 
              char *initenv)
{
    boot_ts = clock();

    sys_environ_init(initenv);

    sys_heap_init();

    sys_fibre_init(fibrebase_init, true);

    sys_vars_init(slrbase_init, true);

    sys_detect_devs();

    sys_argv_init();

    sys_conf_init();

    sys_sep_init();

    sys_places_init();

    sys_check_ncores();

    if (verbose_boot) {
        output_string("* init compiled with slc " __slc_version_string__ ".\n"
                      "\n"
                      "Microgrid says:  h e l l o  w o r l d !\n"
                      "\n"
                      "init done, program will start now; core cycles so far: ", 2); 
        output_uint(clock(), 2);
        output_char('\n', 2);
    }
}
