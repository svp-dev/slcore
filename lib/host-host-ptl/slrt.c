// slrt.c: this file is part of the slc project.
//
// Copyright (C) 2009 The SL project.
// All rights reserved.
//
// $Id$
//
#include "../host-host-seqc/load.c"

extern void lib_main(void);

// configuration tag for sgr
const char *__tag__ = "sgr_runner:host:";

int main(int argc, const char **argv) {
  load(argv[0], argv[1]);
  lib_main();
  return 0;
}
