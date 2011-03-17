//
// mtmalloc.h: this file is part of the SL toolchain.
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
#ifndef SLC_MTA_MTMALLOC_H
#define SLC_MTA_MTMALLOC_H

#include <stddef.h>

/* low-level, thread-unsafe implementations */
void* dlmalloc(size_t);
void dlfree(void*);
void* dlcalloc(size_t, size_t);
void* dlrealloc(void*, size_t);

/* "fast" aliases */
#if defined(__mt_freestanding__) && (defined(__alpha__) || defined(__mtalpha__) || defined(__mtsparc__))

#include <svp/compiler.h>

#define malloc_place 0xf /* 8=suspend, 4|2=delegate, 1=exclusive (implicit core ID = 0) */

sl_decl(t_dlmalloc,,sl_shparm(void*, szret));

alwaysinline
void* excl_dlmalloc(size_t sz)
{
    sl_create(,malloc_place,,,,,sl__exclusive,t_dlmalloc,sl_sharg(void*,szret,(void*)sz));
    sl_sync();
    return (void*)sl_geta(szret);
}

sl_decl(t_dlfree,,sl_glparm(void*, ptr));

alwaysinline
void excl_dlfree(void* ptr)
{
    sl_create(,malloc_place,,,,,sl__exclusive,t_dlfree,sl_glarg(void*,,ptr));
    sl_sync();
}

sl_decl(t_dlrealloc,,sl_glparm(void*, ptr), sl_shparm(void*, szret));

alwaysinline
void* excl_dlrealloc(void* ptr, size_t sz)
{
    sl_create(,malloc_place,,,,,sl__exclusive,t_dlrealloc,sl_glarg(void*,,ptr), sl_sharg(void*,szret,(void*)sz));
    sl_sync();
    return (void*)sl_geta(szret);
}

sl_decl(t_dlcalloc,,sl_glparm(size_t, cnt), sl_shparm(void*, szret));

alwaysinline
void* excl_dlcalloc(size_t cnt, size_t sz)
{
    sl_create(,malloc_place,,,,,sl__exclusive,t_dlcalloc,sl_glarg(size_t,,cnt), sl_sharg(void*,szret,(void*)sz));
    sl_sync();
    return (void*)sl_geta(szret);
}

#endif

#endif
