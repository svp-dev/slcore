//
// va.c: this file is part of the SL toolchain.
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

#include <cstdarg.h>
#include <svp/testoutput.h>

int select(int s, ...)
{
    va_list ap;
    va_start(ap, s);
    int x = va_arg(ap, int);
    int y = va_arg(ap, int);
    va_end(ap);
    return s ? x : y;
}

sl_def(t_main, void)
{
    int a = select(0, 123, 456);
    int b = select(1, 789, 666);

    output_int(a, 1);
    output_char('\n', 1);
    output_int(b, 1);
    output_char('\n', 1);
}
sl_enddef
