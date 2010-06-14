//
// env.c: this file is part of the SL program suite.
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

#include <stdio.h>
#include <stdlib.h>

// XIGNORE: *:D
sl_def(t_main, void)
{
    const char *h = getenv("HOME");
    h = h ? h : "(undefined)";
    printf("before setenv:\nHOME = %s\n", h);

    setenv("HOME", "my new home", 1);
    h = getenv("HOME");
    h = h ? h : "(undefined)";
    printf("after setenv:\nHOME = %s\n", h);
}
sl_enddef
