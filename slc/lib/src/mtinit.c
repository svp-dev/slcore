//
// mtinit.c: this file is part of the SL toolchain.
//
// Copyright (C) 2009,2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//
#include <svp/testoutput.h>
#include <svp/sep.h>
#include <svp/fibre.h>
#include <svp/slr.h>
#include <svp/gomt.h>
#include <cstdlib.h>
#include <cstring.h>

/* some global variables used by programs */
struct fibre_base_t *__fibre_base;
struct __slr_base_t *__slr_base;

static
void environ_init(char *initenv)
{
    extern char** environ;

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

static unsigned req_ncores;
extern sl_place_t __main_place_id;
extern struct placeinfo *__main_placeinfo;
sl_def(__set_main_pid, void)
{
    sl_create(,PLACE_LOCAL,,,,,, root_sep->sep_alloc,
              sl_glarg(struct SEP*, , root_sep),
              sl_glarg(unsigned long, , SAL_EXACT | req_ncores),
              sl_sharg(struct placeinfo*, p, 0));
    sl_sync();
    if (sl_geta(p)) {
        __main_place_id = sl_geta(p)->pid;
        __main_placeinfo = sl_geta(p);
        output_string("# place allocated for t_main: pid=0x", 2);
        output_hex(__main_place_id, 2);
        output_string(", ncores=", 2);
    } else {
        output_string("#warning: cannot allocate place with ncores=", 2);
    }
    output_int(req_ncores, 2);
    output_char('\n', 2);
}
sl_enddef

static
void set_main_pid()
{
    char *v = getenv("SLR_NCORES");
    if (v && v[0]) {
        req_ncores = strtoul(v, 0, 10);
        if (req_ncores)
            (void)INVOKE_MT(PLACE_LOCAL, __set_main_pid);
    }
}

void _lib_init_routine(void* slrbase_init, 
                       void* fibrebase_init, 
                       char *initenv, 
                       void *placeconf)
{
  __slr_base = (struct __slr_base_t*) slrbase_init;
  __fibre_base = (struct fibre_base_t*) fibrebase_init;
  environ_init(initenv);
  sep_init(placeconf);
  set_main_pid();
}
