// slrt.c: this file is part of the slc project.
//
// Copyright (C) 2009 The SL project.
// All rights reserved.
//
// $Id$
//
extern void lib_main(void);

// configuration tag for sgr
const char *__tag__ = "sgr_runner:host:";

int main(void) {
  lib_main();
  return 0;
}
