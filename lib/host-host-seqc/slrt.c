//
// seqc-main.c: this file is part of the slc project.
//
// Copyright (C) 2008 The CSA group.
//
// $Id$
//
#include "include/sl_support.h"

extern void t_main(struct sl_famdata*);

// configuration tag for sgr runner
const char *__tag__ = "sgr_runner:host:";

int main(void) {
  struct { } args;
  struct sl_famdata root;
  root.ix = root.be = 0;
  root.li = root.st = 1;
  root.f = t_main;
  root.a = &args;
  root.ex = 0;
  t_main(&root);
  return 0;
}
