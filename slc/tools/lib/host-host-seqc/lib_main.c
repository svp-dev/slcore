//
// lib_main.c: this file is part of the SL toolchain.
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

extern void t_main(struct sl_famdata*);

void lib_main(void) {
  struct { } args;
  struct sl_famdata root;
  root.ix = root.be = 0;
  root.li = root.st = 1;
  root.f = t_main;
  root.a = &args;
  root.ex = 0;
  t_main(&root);
}
