//
// fptr.c: this file is part of the SL toolchain.
//
// Copyright (C) 2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//
#include <svp/testoutput.h>

int e = 0;
int f = 0;

sl_def(foo, void)
{
    e++;
}
sl_enddef

sl_def(foo2, sl__naked(native))
{
    f++;
}
sl_enddef

sl_decl_fptr(blah, void);
sl_decl_fptr(blah2, sl__naked(native));

int main(void)
{
   sl_decl_fptr(blih, void);
   blih = &foo;
   sl_decl_fptr(blih2, sl__naked(native));
   blih2 = &foo2;
   blah = blih;
   blah2 = blih2;
   sl_create(,,,,,,, *blih); sl_sync();
   sl_create(,,,,,,sl__naked(native), *blih2); sl_sync();
   sl_create(,,,,,,, *blah); sl_sync();
   sl_create(,,,,,,sl__naked(native), *blah2); sl_sync();
   sl_create(,,,,,,, foo); sl_sync();
   sl_create(,,,,,,sl__naked(native), foo2); sl_sync();

   return !(e == 2 && f == 2);
}
