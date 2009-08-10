//
// minimath.h: this file is part of the SL toolchain.
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

#ifndef SLC_SVP_MINIMATH_H
# define SLC_SVP_MINIMATH_H

#ifdef __mt_freestanding__

typedef float float_t;
typedef double double_t;

#define FLT_EVAL_METHOD      0

#define HUGE_VAL (__extension__ 0x1.0p2047)
#define HUGE_VALF (__extension__ 0x1.0p255f)
#define HUGE_VALL HUGE_VAL

#define INFINITY     HUGE_VALF

# define NAN								m4_dnl
  (__extension__							m4_dnl
   ((union { unsigned __l __attribute__((__mode__(__SI__))); float __d; }) m4_dnl
    { __l: 0x7fc00000UL }).__d)

# define FP_ILOGB0     (-2147483647)
# define FP_ILOGBNAN   (2147483647)

/* the following functions inline to assembly on the Alpha */

extern double sqrt (double __x) ; extern double __sqrt (double __x) ;
extern float sqrtf (float __x) ; extern float __sqrtf (float __x) ;

extern float fabsf (float __x) __attribute__ ((__const__));
extern float __fabsf (float __x) __attribute__ ((__const__));
extern double fabs (double __x) __attribute__ ((__const__));
extern double __fabs (double __x) __attribute__ ((__const__));

extern float truncf (float __x) __attribute__ ((__const__));
extern float __truncf (float __x) __attribute__ ((__const__));
extern double trunc (double __x) __attribute__ ((__const__));
extern double __trunc (double __x) __attribute__ ((__const__));

extern float copysignf (float __x, float __y) __attribute__ ((__const__));
extern float __copysignf (float __x, float __y) __attribute__ ((__const__));
extern double copysign (double __x, double __y) __attribute__ ((__const__));
extern double __copysign (double __x, double __y) __attribute__ ((__const__));

extern float fminf (float __x, float __y) ; extern float __fminf (float __x, float __y) ;
extern double fmin (double __x, double __y) ; extern double __fmin (double __x, double __y) ;
extern float fmaxf (float __x, float __y) ; extern float __fmaxf (float __x, float __y) ;
extern double fmax (double __x, double __y) ; extern double __fmax (double __x, double __y) ;

#else
#include <math.h>
#endif

#endif // ! SLC_SVP_MINIMATH_H
