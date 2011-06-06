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

int verbose_boot = 1;
clock_t boot_ts = 0;

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
void sys_set_main_pid(unsigned req_ncores)
{
    if (verbose_boot) output_string(" -> request to SEP...", 2);
    int r = sep_alloc(root_sep, &__main_place_id, SAL_EXACT, req_ncores);
    if (r == -1) 
    {
        if (verbose_boot)
            output_string(" failed", 2);
        abort();
    }
    if (verbose_boot) 
    {
        output_string(" success, t_main goes to pid=0x", 2);
        output_hex(__main_place_id, 2);
        output_ts(2);
        output_char('\n', 2);
    }
}

static noinline
void sys_check_ncores(void)
{
    char *v = getenv("SLR_NCORES");
    if (v && v[0]) {
        if (verbose_boot) {
            output_string("* -n ", 2);
            output_string(v, 2);
        }
        unsigned req_ncores = strtoul(v, 0, 10);
        if (req_ncores)
            sys_set_main_pid(req_ncores);
        else
            if (verbose_boot) 
                output_string(" (invalid, ignoring)\n", 2);
    }
}

extern void sys_sep_init(void);
extern void sys_fibre_init(void*, bool);
extern void sys_vars_init(void*, bool);

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

    sys_conf_init();

    sys_sep_init();

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
