//
// load.c: this file is part of the SL toolchain.
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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static 
void fail(const char *progname, const char *where)
{
    if (progname)
        fprintf(stderr, "%s: ", progname);
    perror(where);
    exit(1);
}

static
void load_binary_data(const char *progname, const char* fname, void**ptr, bool verbose)
{
    FILE *f;

    *ptr = 0;

    if (!fname) {
        if (progname) fprintf(stderr, "%s: ", progname);
        fprintf(stderr, "warning: no data file specified (did you use slr?)\n");
        return ;
    }
    if (verbose) fprintf(stderr, "# loading data from %s...\n", fname);

#define ensure(Cond, What) do { if (!(Cond)) fail(progname, What); } while(0);

    ensure(f = fopen(fname, "rb"), "fopen");

    ensure(fseek(f, 0L, SEEK_END) == 0, "fseek");
    
    long sz = ftell(f);
    ensure(sz >= 0, "ftell");
    
    if (sz > 0) {
        void *p;
        ensure(p = malloc(sz + sizeof(double)), "malloc"); // ensure enough space after end
        
        fseek(f, 0L, SEEK_SET);
        ensure(fread(p, sz, 1, f) == 1, "fread");
        *ptr = p;
        if (verbose) fprintf(stderr, "# %ld bytes loaded\n", sz);
    }
    fclose(f);
}


#ifdef __cplusplus
extern "C" {
#endif

void *__slr_base = 0;
void *__fibre_base = 0;

__attribute__((__constructor__))
void slr_init(
#ifndef __cplusplus
int argc, char **argv
#endif
)
{
    char *progname = 0;
#ifndef __cplusplus
    if (argc && argv[0][0])
        progname = argv[0];
#endif
    char *vs = getenv("VERBOSE");
    bool verbose = false;
    if (vs && vs[0])
        verbose = true;
    
    char *data = getenv("SLR_DATA");
    if (data)
        load_binary_data(progname, data, &__slr_base, verbose);

    char *fdata = getenv("SLR_FDATA");
    if (fdata)
        load_binary_data(progname, fdata, &__fibre_base, verbose);    
}

#ifdef __cplusplus
}
#endif
