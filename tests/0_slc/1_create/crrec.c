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

// We can't recurse too deep on _n targets because
// of thread exhaustion.
// XIGNORE: mt*_*n:R

// We can't recurse too deep on 32-bit MTSPARC, which
// will have a tiny stack on its main thread.
#if defined(__slc_arch_mtsparc__) && defined(__slc_variant_seq_naked__)
# define MAX_REC 10
#else
# define MAX_REC 200
#endif
// XIGNORE: mts*_s:D

sl_def(foo,void,sl_glparm(int, x))
{
    if (sl_getp(x) > 0)
    {
        sl_create(,,,,,,,foo,sl_glarg(int,,sl_getp(x)-1));
        sl_sync();
    }
    output_char('.', 1);
}
sl_enddef

sl_def(t_main,void)
{
    sl_create(,,,,,,,foo,sl_glarg(int,,MAX_REC));
    sl_sync();
    output_char('\n', 1);
}
sl_enddef
