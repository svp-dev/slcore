//
// load.c: this file is part of the SL toolchain.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void *__slr_base;
void *__fibre_base;

void fail(const char *progname, const char *where)
{
  fprintf(stderr, "%s: %s: %s\n", progname, where, strerror(errno));
  exit(1);
}

void load(const char *progname, const char* fname, void**ptr)
{
  if (!fname) {
    fprintf(stderr, "%s: warning: no data file specified (did you use slr?)\n", progname);
    *ptr = 0;
    return ;
  }
  FILE *f;
  if (!(f = fopen(fname, "rb"))) fail(progname, "fopen");
  fseek(f, 0L, SEEK_END);

  long sz = ftell(f);
  if (sz < 0) fail(progname, "ftell");
  /*  else if (!sz) {
    fprintf(stderr, "%s: %s: data file is empty\n", progname, fname);
    exit(1);
    } */

  if (sz > 0) {
    void *p;
    if (!(p = malloc(sz))) fail(progname, "malloc");

    fseek(f, 0L, SEEK_SET);
    if (0 == fread(p, sz, 1, f)) fail(progname, "fread");
    *ptr = p;
  }
  else
    *ptr = 0;
  fclose(f);
}
