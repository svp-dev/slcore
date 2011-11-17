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

#include <svp/testoutput.h>
#include <time.h>

#include "mtinit.h"
#include "mgsim/mtconf.h"
#include "posix-compat/heap.h"

void sys_init(void* slrbase_init, 
              void* fibrebase_init, 
              char *initenv)
{
    boot_ts = clock();

#if !defined(__slc_arch_host__)

    /* the following initialization only apply for the Microgrid
     * hardware platforms. */

    sys_environ_init(initenv);

    sys_heap_init();

    sys_fibre_init(fibrebase_init, true);

    sys_vars_init(slrbase_init, true);

    sys_detect_devs();

    sys_argv_init();

    sys_conf_init();

#endif

    sys_sep_init();

#if !defined(__slc_arch_host__) || \
  defined(__slc_os_host_hlsim__) || defined(__slc_os_host_hlsimd__)

    sys_places_init();

#endif

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
