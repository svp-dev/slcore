//
// argshadow.c: this file is part of the SL toolchain.
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

sl_def(foo, void, sl_glparm(int, a)) {} sl_enddef

struct blah { int x; } a;
int main(void)
{
  int z;
  if (1) {
    sl_create(,,,,,,,foo, sl_glarg(int, a, 10)); // valid: different scope
    sl_sync();
    z = sl_geta(a); // a still visible
  }
  a.x = 20; // previous a now visible
  return !(a.x == 20 && z == 10);
}

