//
// mtargs.c: this file is part of the SL toolchain.
//
// Copyright (C) 2011 Universiteit van Amsterdam.
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
#include <stdlib.h>
#include <string.h>

#include "mgsim/mtconf.h"
#include "mgsim/mgsim.h"
#include "malloc/mtmalloc.h"

extern long __argc;
extern char** __argv_ptr;
void sys_argv_init(void)
{
    /* attempt to find the argv ROM */
    if (mg_argvrom_devid == (size_t)-1)
    {
        if (verbose_boot)
        {
            output_string("* no argument ROM, unable to read arguments", 2);
            output_ts(2);
            output_char('\n', 2);
        }
        return;
    }

    const struct argvdata {
        uint32_t magic;
        uint32_t argc;
        uint32_t datasize;
        char data[];
    } *argvdata = (const struct argvdata*)mg_devinfo.base_addrs[mg_argvrom_devid];

    size_t argc = argvdata->argc;
    size_t datasize = argvdata->datasize;

    char *localdata = (char*)dlmalloc(datasize);
    char** argv = (char**)dlmalloc((argc + 1) * sizeof(char*));
    if (localdata == NULL || argv == NULL)
    {
        output_string("Unable to allocate memory to copy the argument data!\n", 2);
        abort();
    }

    memcpy(localdata, argvdata->data, datasize);
    
    char *p;
    size_t i;
    for (i = 0, p = localdata; i < argc; ++i)
    {
        argv[i] = p;
        p = p + strlen(p) + 1;
    }
    argv[i] = 0;

    __argc = argc;
    __argv_ptr = argv;

    if (verbose_boot)
    {
        output_string("* input data: arguments at 0x", 2);
        output_hex(localdata, 2);
        output_string(", argv[", 2);
        output_uint(argc, 2);
        output_string("] at 0x", 2);
        output_hex(argv, 2);
        output_string(", ", 2);
        output_uint(datasize, 2);
        output_string(" bytes.", 2);
        output_ts(2);
        output_char('\n', 2);
    }
}
