//
// createinfun.c: this file is part of the SL toolchain.
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
#include <svp/testoutput.h>

sl_def(bar, void, sl_shparm(int, x))
{
    sl_setp(x, sl_getp(x)+1);
}
sl_enddef

int foo(int x) {
    
    sl_create(,,,,,,, bar, sl_sharg(int, xs, x));
    sl_sync();
    return sl_geta(xs);

}

sl_def(t_main, void)
{
    output_int(foo(41), 1);
    output_char('\n', 1);
}
sl_enddef
