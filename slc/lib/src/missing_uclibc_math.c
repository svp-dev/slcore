//
// missing_uclibc_math.c: this file is part of the SL toolchain.
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

#include <cmath.h>

int errno;

extern double __cos(double) __asm__("cos");
extern double __cosf(float) __asm__("cos");
extern double __sin(double) __asm__("sin");
extern double __sinf(float) __asm__("sinf");

#undef sincos
void sincos(double x, double* s, double *c) {
  *s = __sin(x);
  *c = __cos(x);
}

#undef sincosf
void sincosf(float x, float* s, float *c) {
  *s = __sinf(x);
  *c = __cosf(x);
}

#undef exp2f
#undef exp2
extern double __exp2(double) __asm__("exp2");
float exp2f(float x) { return __exp2(x); }
#undef log2
#undef log2f
extern double __log2(double) __asm__("log2");
float log2f(float x) { return __log2(x); }
#undef nearbyint
#undef nearbyintf
extern double __nearbyint(double) __asm__("nearbyint");
float nearbyintf(float x) { return __nearbyint(x); }
#undef remquo
#undef remquof
float remquof(float x, float y, int *quo) { return remquo(x, y, quo); }
#undef fdim
#undef fdimf
float fdimf(float x, float y) { return fdim(x, y); }
#undef fmax
#undef fmaxf
float fmaxf(float x, float y) { return fmax(x, y); }
#undef fmin
#undef fminf
float fminf(float x, float y) { return fmin(x, y); }
#undef fma
#undef fmaf
float fmaf(float x, float y, float z)  { return fma(x, y, z); }
