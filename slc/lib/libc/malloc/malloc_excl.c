//
// malloc_excl.c: this file is part of the SL toolchain.
//
// Copyright (C) 2011 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include <stdlib.h>
#include <svp/mtmalloc.h>

sl_def(t_dlmalloc,,sl_shparm(void*, szret))
{
    size_t sz = (size_t)sl_getp(szret);
    sl_setp(szret, (void*)dlmalloc(sz));
}
sl_enddef

sl_def(t_dlfree,,sl_glparm(void*, ptr))
{
    dlfree(sl_getp(ptr));
}
sl_enddef

sl_def(t_dlcalloc,,sl_glparm(size_t, cnt), sl_shparm(void*, szret))
{
    size_t sz = (size_t)sl_getp(szret);
    sl_setp(szret, (void*)dlcalloc(sl_getp(cnt), sz));
}
sl_enddef

sl_def(t_dlrealloc,,sl_glparm(void*, ptr), sl_shparm(void*, szret))
{
    size_t sz = (size_t)sl_getp(szret);
    sl_setp(szret, (void*)dlrealloc(sl_getp(ptr), sz));
}
sl_enddef

