//
// slrt.c: this file is part of the slc project.
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
// $Id$
//

#include "load.c"

extern void t_main(struct sl_famdata*);

// configuration tag for slr runner
const char *__tag__ = "slr_runner:host:";
const char *__datatag__ = "slr_datatag:seqc-host-host-seqc:";

int main(int argc, const char **argv) {
  load(argv[0], argv[1], &__slr_base);
  load(argv[0], argv[2], &__fibre_base); 
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
