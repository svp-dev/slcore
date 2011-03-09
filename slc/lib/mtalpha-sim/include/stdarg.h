//
// stdarg.h: this file is part of the SL toolchain.
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
#ifndef SLC_MTA_STDARG_H
#define SLC_MTA_STDARG_H

#if defined(__GNUC__) && !defined(__AVOID_GNUISMS)
typedef __builtin_va_list va_list;
#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)
#define va_copy(d,s)    __builtin_va_copy(d,s)
#define __va_list_available
#else
# if defined(__COSY_COMPILER)
typedef void **va_list;
[[#]]pragma ckf __builtin_va_start va_start f
extern void **__builtin_va_start(void);
#  define va_start(ap, lastfixed) \
    ((void)((ap) = __builtin_va_start()))
#  define va_arg(ap, type) (*(type*)*(ap)++)
#  define va_end(ap)       ((void)(ap))
#  define __va_list_available
#else
# warning "Cannot define va_ primitives for this target."
#endif
#endif

#endif
