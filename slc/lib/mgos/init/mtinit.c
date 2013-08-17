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
#include <sys/time.h>
#include <time.h>

#include "mtinit.h"
#include "mgsim/mtconf.h"
#include "posix-compat/heap.h"

static
struct timeval main_start_time;
static
struct timeval empty_time;
static
clock_t main_start_clock;

void sys_init(void* fibrebase_init,
              char *initenv)
{
    boot_ts = clock();

#if !defined(__slc_arch_host__)

    /* the following initialization only apply for the Microgrid
     * hardware platforms. */

    sys_environ_init(initenv);

    sys_heap_init();

    sys_fibre_init(fibrebase_init, true);

    sys_detect_devs();

    sys_argv_init();

    sys_conf_init();

#endif

    sys_sep_init();

#if defined(__slc_multiple_places__)

    sys_places_init();

    sys_check_ncores();

#endif

    if (verbose_boot) {

#ifdef __slc_os_sim__
        gettimeofday_virtual(&empty_time, 0);
        gettimeofday_virtual(&empty_time, 0); // two times to warm up cache
        gettimeofday_virtual(&main_start_time, 0);
#else
        gettimeofday(&empty_time, 0);
        gettimeofday(&empty_time, 0); // two times to warm up cache
        gettimeofday(&main_start_time, 0);
#endif

        output_string("* init compiled with slc " __slc_version_string__ ".\n"
                      "\n"
                      "Microgrid says:  h e l l o  w o r l d !\n"
                      "\n"
                      "init done, program will start now; core cycles so far: ", 2);
        output_uint(main_start_clock = clock(), 2);
        output_char('\n', 2);
    }
}

int sys_cleanup(int main_status_code)
{
    if (verbose_boot) {
        clock_t c_end = clock();
        struct timeval tv_end;
#ifdef __slc_os_sim__
        gettimeofday_virtual(&tv_end, 0);
#else
        gettimeofday(&tv_end, 0);
#endif
        unsigned long usecs_empty = 1000000 * (main_start_time.tv_sec - empty_time.tv_sec) +
            (main_start_time.tv_usec - empty_time.tv_usec);

        unsigned long usecs = 1000000 * (tv_end.tv_sec - main_start_time.tv_sec) +
            (tv_end.tv_usec - main_start_time.tv_usec);

        usecs = usecs - usecs_empty;

        output_string("main() completed in ", 2);
        output_uint(usecs / 1000000, 2);
        output_string(" seconds and ",2);
        output_uint(usecs % 1000000, 2);
        output_string(" microseconds (", 2);
        output_uint(c_end - main_start_clock, 2);
        output_string(" clock ticks), with status ", 2);
        output_int(main_status_code, 2);
        output_char('\n', 2);
    }
    return main_status_code;
}
