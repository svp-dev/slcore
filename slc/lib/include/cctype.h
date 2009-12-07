//
// cfloat.h: this file is part of the SL toolchain.
//
// Copyright (C) 2009 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#ifndef SLC_CCTYPE_H
# define SLC_CCTYPE_H

#ifdef __mt_freestanding__

#define _U      1
#define _L      2
#define _N      4
#define _S      8
#define _P      16
#define _C      32
#define _X      64
#define _B      128

extern const char * const restrict __ctype__;

/* access macros */
#define __isalnum_impl(C) (__ctype__[C] & (_U|_L|_N))
#define __isalpha_impl(C) (__ctype__[C] & (_U|_L))
#define __isblank_impl(C) (__ctype__[C] & (_B))
#define __iscntrl_impl(C) (__ctype__[C] & (_C))
#define __isdigit_impl(C) (__ctype__[C] & (_N))
#define __isgraph_impl(C) (__ctype__[C] & (_P|_U|_L|_N))
#define __islower_impl(C) ((__ctype__[C] & (_U|_L)) == _L)
#define __isprint_impl(C) (__ctype__[C] & (_P|_U|_L|_N|_B))
#define __ispunct_impl(C) (__ctype__[C] & (_P))
#define __isspace_impl(C) (__ctype__[C] & (_S))
#define __isupper_impl(C) ((__ctype__[C] & (_U|_L)) == _U)
#define __isxdigit_impl(C) (__ctype__[C] & (_X|_N))
#define __tolower_impl(C) ({ int __c = (C); isupper(__c) ? (__c - 'A' + 'a') : __c; })
#define __toupper_impl(C) ({ int __c = (C); islower(__c) ? (__c - 'a' + 'A') : __c; })
#define __isascii_impl(C) ((unsigned)(C) <= 127)
#define __toascii_impl(C) ((C) & 127)

/* function prototypes; these must exist */
int isalnum(int c);
int isalpha(int c);
int isblank(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int tolower(int c);
int toupper(int c);
int isascii(int c);
int toascii(int c);

/* macro overloads that can be inlined */
#define isalnum(C) __isalnum_impl(C)
#define isalpha(C) __isalpha_impl(C)
#define isblank(C) __isblank_impl(C)
#define iscntrl(C) __iscntrl_impl(C)
#define isdigit(C) __isdigit_impl(C)
#define isgraph(C) __isgraph_impl(C)
#define islower(C) __islower_impl(C)
#define isprint(C) __isprint_impl(C)
#define ispunct(C) __ispunct_impl(C)
#define isspace(C) __isspace_impl(C)
#define isupper(C) __isupper_impl(C)
#define isxdigit(C) __isxdigit_impl(C)
#define tolower(C) __tolower_impl(C)
#define toupper(C) __toupper_impl(C)
#define isascii(C) __isascii_impl(C)
#define toascii(C) __toascii_impl(C)

#else
#ifdef __cplusplus
#include <cctype>
#else
#include <ctype.h>
#endif
#endif

#endif // ! SLC_CCTYPE_H
