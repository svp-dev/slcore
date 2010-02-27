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
#include <svp/compiler.h>
#include <svp/testoutput.h>
#include <svp/sep.h>
#include <svp/fibre.h>
#include <svp/slr.h>
#include <svp/gomt.h>
#include <cstdlib.h>
#include <cstring.h>
#include <ctime.h>
#include "mtconf.h"
#include "heap.h"

/* some global variables used by programs */
struct fibre_base_t *__fibre_base;
struct __slr_base_t *__slr_base;

int verbose_boot;

static noinline
void sys_environ_init(char *initenv)
{
    extern char** environ;

    /* 
       the environment is provided as concatenated C strings separated
       by an ASCII NUL (0). The last string is followed by a
       a couple of standalone NUL bytes.
    */

    output_string("# env: scan...", 2);
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

    output_string(" read...", 2);
    environ = dlmalloc((nvars + 1) * sizeof(char*));
    size_t i;
    for (i = 0, p = initenv; i < nvars; ++i, p += strlen(p) + 1)
    {
        environ[i] = p;
    }
    environ[i] = 0;
    output_string(" done.\n", 2);
}

static unsigned sys_req_ncores;
extern sl_place_t __main_place_id;
extern struct placeinfo *__main_placeinfo;

sl_def(sys_set_main_pid, void)
{
    if (verbose_boot) output_string(" -> request to SEP...", 2);
    sl_create(,PLACE_LOCAL,,,,,, root_sep->sep_alloc,
              sl_glarg(struct SEP*, , root_sep),
              sl_glarg(unsigned long, , SAL_EXACT | sys_req_ncores),
              sl_sharg(struct placeinfo*, p, 0));
    sl_sync();
    if (sl_geta(p)) {
        __main_place_id = sl_geta(p)->pid;
        __main_placeinfo = sl_geta(p);
        if (verbose_boot) 
            output_string(" success", 2);
    } else {
        if (verbose_boot) output_string(" failed", 2);
    }
    if (verbose_boot) 
    {
        output_string(", t_main goes to pid=0x", 2);
        output_hex(__main_place_id, 2);
        output_string(", ncores=", 2);
        output_int(__main_placeinfo->ncores, 2);
        output_char('\n', 2);
    }
}
sl_enddef

static noinline
void sys_check_ncores(void)
{
    char *v = getenv("SLR_NCORES");
    if (v && v[0]) {
        if (verbose_boot) {
            output_string("* -n ", 2);
            output_string(v, 2);
        }
        sys_req_ncores = strtoul(v, 0, 10);
        if (sys_req_ncores)
            (void)INVOKE_MT(PLACE_LOCAL, sys_set_main_pid);
        else
            if (verbose_boot) 
                output_string(" (invalid, ignoring)\n", 2);
    }
}

static noinline
void sys_vars_init(void *slrbase_init, void *fibrebase_init)
{
  if (verbose_boot) {
      output_string("* input data: fibre at 0x", 2);
      output_hex(slrbase_init, 2);
      output_string(", slr vars at 0x", 2);
      output_hex(slrbase_init, 2);
      output_char('.', 2);
      output_char('\n', 2);
  }
  __slr_base = (struct __slr_base_t*) slrbase_init;
  __fibre_base = (struct fibre_base_t*) fibrebase_init;
}

void sys_init(void* slrbase_init, 
              void* fibrebase_init, 
              char *initenv, 
              void *confdata)
{
  sys_environ_init(initenv);

  verbose_boot = getenv("MGSYS_QUIET") ? 0 : 1;

  if (verbose_boot) {
      output_string("\nMicrogrid says:  h e l l o  w o r l d !\n\n", 2);
  }
  
  sys_heap_init();

  sys_vars_init(slrbase_init, fibrebase_init);

  sys_conf_init(confdata);

  sys_sep_init(mgconf_layout_data);

  sys_check_ncores();

  if (verbose_boot) 
  {
      output_string("\ninit done, ", 2); 
      output_uint(clock(), 2);
      output_string(" cycles wasted. program will start now.\n\n", 2);
  }
}
