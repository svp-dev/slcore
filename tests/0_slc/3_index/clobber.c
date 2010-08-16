//
// clobber.c: this file is part of the SL toolchain.
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
#include <svp/compiler.h>
#include <stddef.h>

noinline
size_t bar(const char* s)
{
    size_t l;
    for (l = 0; *s; ++l, ++s) ;
    return l;
}

sl_def(foo, void, sl_shparm(int, token))
{
    int token = sl_getp(token);
    (void)bar("");
    sl_index(i);
    output_int(i, 1); output_char('\n', 1);
    sl_setp(token, token);
}
sl_enddef

sl_def(t_main, void)
{
    sl_create(,,0,4,,,, foo,
        sl_sharg(int, token, 0)
    );
    sl_sync();
}
sl_enddef
