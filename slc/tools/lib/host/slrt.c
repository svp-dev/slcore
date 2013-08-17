//
// slrt.c: this file is part of the SL toolchain.
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

#include <svp/delegate.h>

unsigned long boot_ts = 0;
int verbose_boot = 1;
sl_place_t __main_place_id = 0;

const char *__tag__ = "\0slr_runner:host:";
const char *__datatag__ = "\0slr_datatag:native:";

#include "load.c"

#if defined(__slc_os_host_ptl__) || defined(__slc_os_host_hlsim__) \
    || defined(__slc_os_host_ptld__) || defined(__slc_os_host_hlsimd__)

/* utc-ptl and hlsim define their own "main" function which initialize the ptl
   library, then creates an initial family running a function called "_main". However
   the input SL code may also require to define a plain C "main" function and expect
   that to be run in a thread. 

   So instead we rename all occurrences of "main" in the SL code to "Pmain", and
   we use that from here.
*/

// defined by program
extern int Pmain(int, char**);

extern int __main_exit_status;

// Wrapper from an SVP-PTL style thread function to the
// user level main function with the original arguments.
void _main(void)
{
    int argc;
    char **argv, **envp;
    uTC::args_get(&argc, &argv, &envp);

    sys_init(NULL, NULL);

    __main_exit_status = Pmain(argc, argv);
}

#endif // ptl || hlsim
