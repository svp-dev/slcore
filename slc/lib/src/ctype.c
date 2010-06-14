//
// ctype.c: this file is part of the SL toolchain.
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

#include <ctype.h>

#undef isalnum
#undef isalpha
#undef isblank
#undef iscntrl
#undef isdigit
#undef isgraph
#undef islower
#undef isprint
#undef ispunct
#undef isspace
#undef isupper
#undef isxdigit
#undef toupper
#undef tolower
#undef isascii
#undef toascii

int isalnum(int c) { return __isalnum_impl(c); }
int isalpha(int c) { return __isalpha_impl(c); }
int isblank(int c) { return __isblank_impl(c); }
int iscntrl(int c) { return __iscntrl_impl(c); }
int isdigit(int c) { return __isdigit_impl(c); }
int isgraph(int c) { return __isgraph_impl(c); }
int islower(int c) { return __islower_impl(c); }
int isprint(int c) { return __isprint_impl(c); }
int ispunct(int c) { return __ispunct_impl(c); }
int isspace(int c) { return __isspace_impl(c); }
int isupper(int c) { return __isupper_impl(c); }
int isxdigit(int c) { return __isxdigit_impl(c); }
int toupper(int c) { return __toupper_impl(c); }
int tolower(int c) { return __tolower_impl(c); }
int isascii(int c) { return __isascii_impl(c); }
int toascii(int c) { return __toascii_impl(c); }

#define ctype_0_127 \
    _C,     _C,     _C,     _C,     _C,     _C,     _C,     _C, \
    _C,     _C|_S|_B, _C|_S, _C|_S,    _C|_S,  _C|_S,  _C,     _C, \
    _C,     _C,     _C,     _C,     _C,     _C,     _C,     _C, \
    _C,     _C,     _C,     _C,     _C,     _C,     _C,     _C, \
    _S|_B,  _P,     _P,     _P,     _P,     _P,     _P,     _P, \
    _P,     _P,     _P,     _P,     _P,     _P,     _P,     _P, \
    _N,     _N,     _N,     _N,     _N,     _N,     _N,     _N, \
    _N,     _N,     _P,     _P,     _P,     _P,     _P,     _P, \
    _P,     _U|_X,  _U|_X,  _U|_X,  _U|_X,  _U|_X,  _U|_X,  _U, \
    _U,     _U,     _U,     _U,     _U,     _U,     _U,     _U, \
    _U,     _U,     _U,     _U,     _U,     _U,     _U,     _U, \
    _U,     _U,     _U,     _P,     _P,     _P,     _P,     _P, \
    _P,     _L|_X,  _L|_X,  _L|_X,  _L|_X,  _L|_X,  _L|_X,  _L, \
    _L,     _L,     _L,     _L,     _L,     _L,     _L,     _L, \
    _L,     _L,     _L,     _L,     _L,     _L,     _L,     _L, \
    _L,     _L,     _L,     _P,     _P,     _P,     _P,     _C
#define ctype_128_255 \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0

static const char ctype_data[128 + 256] = {
    /* allow for negative indices */
    ctype_128_255,
    ctype_0_127,
    ctype_128_255
};

const char * const restrict __ctype__ = ctype_data + 128;


