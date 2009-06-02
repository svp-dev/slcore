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

#include "../host-host-seqc/load.c"

extern void lib_main(void);

// configuration tag for slr
const char *__tag__ = "slr_runner:host:";
const char *__datatag__ = "slr_datatag:ptl-host-host-ptl:";

int main(int argc, const char **argv) {
  load(argv[0], argv[1]);
  lib_main();
  return 0;
}
