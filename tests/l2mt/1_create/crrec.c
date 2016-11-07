//
// sac1.c: this file is part of the SL toolchain.
//
// Copyright (C) 2009-2015 The SL project.
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

# define MAX_REC 5

int e = 0;

sl_def(foo,void,sl_glparm(int, x))
{
    if (sl_getp(x) > 0)
    {
        sl_create(,,,,,,,foo,sl_glarg(int,,sl_getp(x)-1));
        sl_sync();
    } else {
	e = 1;
    }
}
sl_enddef

int main(void)
{
    sl_create(,,,,,,,foo,sl_glarg(int,,MAX_REC));
    sl_sync();
    return e - 1;
}

