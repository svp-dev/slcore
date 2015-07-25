//
// bigarray2.c: this file is part of the SL toolchain.
//
// Copyright (C) 2010-2015 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include <svp/compiler.h>
#include <svp/testoutput.h>

noinline
int foo(int*a) { barrier(); return a[42]; }

// 2015-07-24: ptl and mts-seq have tiny stacks.
// XIGNORE: ptl*:R
// XIGNORE: mts*_s:R

noinline
int bar(void)
{
    int a[4000];
    a[42] = 123;
    return foo(a)-23-58;
}

sl_def(t_main, void)
{
    output_int(bar(), 1);
    output_char('\n', 1);
}
sl_enddef
