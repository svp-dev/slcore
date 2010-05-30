//
// fptr.c: this file is part of the SL toolchain.
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

#if !defined(A) && !defined(B) && !defined(C) && !defined(D) && !defined(E) && !defined(F)
#define A
#define B
#define C
#define D
#define E
#define F
#endif


#if defined(A) || defined(C) || defined(E)
sl_def(foo, void)
{
 output_string("foo\n", 1);
}
sl_enddef
#endif

#if defined(B) || defined(D) || defined(F)
sl_def(foo2, sl__naked(native))
{
 output_string("foo2\n", 1);
}
sl_enddef
#endif

#if defined(C)
sl_decl_fptr(blah, void);
#endif
#if defined(D)
sl_decl_fptr(blah2, sl__naked(native));
#endif

sl_def(t_main, void)
{
#if defined(A) || defined(C)
   sl_decl_fptr(blih, void);
   blih = &foo;
#endif
#if defined(B) || defined(D)
   sl_decl_fptr(blih2, sl__naked(native));
   blih2 = &foo2;
#endif

#ifdef C 
   blah = blih;
#endif
#ifdef D
   blah2 = blih2;
#endif

#ifdef A
   sl_create(,,,,,,, *blih); sl_sync();
#endif
#ifdef B
   sl_create(,,,,,,sl__naked(native), *blih2); sl_sync();
#endif
#ifdef C
   sl_create(,,,,,,, *blah); sl_sync();
#endif
#ifdef D
   sl_create(,,,,,,sl__naked(native), *blah2); sl_sync();
#endif
#ifdef E
   sl_create(,,,,,,, foo); sl_sync();
#endif
#ifdef F
   sl_create(,,,,,,sl__naked(native), foo2); sl_sync();
#endif
}
sl_enddef
