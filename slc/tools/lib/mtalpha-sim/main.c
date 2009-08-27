//
// main.c: this file is part of the SL toolchain.
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

extern void t_main(void);

int main(void) {
  register void *pv __asm__("$1") = &t_main;
  long ret;
  __asm__ ("allocate %0, 1, 0, 0, 0\n\tcrei %0, 0(%1)"
	   : "=r"(ret), "=r"(pv) : "0"(0), "1"(pv));
  return ret;
}
