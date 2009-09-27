//
// avail.c: this file is part of the SL toolchain.
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

#include <svp/testoutput.h>
#include <cmath.h>
#include <svp/slr.h>

typedef struct {
  double d;
  float f;
  int i;
  long l;
  long long ll;
  const char *desc;
} data_t;

#define MAX_TESTS 1000
data_t values[MAX_TESTS];
int i = 0, p = 0;

sl_def(do_test, void)
{
  int j;
#define x values[p].d
#define xf values[p].f
#define y values[p+1].d
#define yf values[p+1].f
#define n values[p+2].i
#define nl values[p+2].l

#define call1(F)				\
  values[i].desc = #F;				\
  values[i++].d = F(x); p++;			\
  values[i].desc = #F "f";			\
  values[i++].f = F ## f (xf); p++;
#define call1i(F)				\
  values[i].desc = #F;				\
  values[i++].i = F(x); p++

#define call2(F)				\
  values[i].desc = #F;				\
  values[i++].d = F(x, y); p+=2;		\
  values[i].desc = #F "f";			\
  values[i++].f = F ## f (xf, yf); p+=2
#define call2i(F)				\
  values[i].desc = #F;				\
  values[i++].i = F(x, y); p+=2;		\
  values[i].desc = #F "f";			\
  values[i++].i = F(xf, yf); p+=2

  /* classify */
  call1i(fpclassify);
  call1i(signbit);
  call1i(isfinite);
  call1i(isnormal);
  call1i(isnan);
  call1i(isinf);

  /* trig */
  call1(acos);
  call1(asin);
  call1(atan);
  call2(atan2);
  call1(cos);
  call1(sin);
  call1(tan);

  /* hyperbolic */
  call1(acosh);
  call1(asinh);
  call1(atanh);
  call1(cosh);
  call1(sinh);
  call1(tanh);

  /* exp/log */
  call1(exp);
  call1(exp2);
  call1(expm1);

  j = i++;
  values[j].desc = "frexp";
  values[j].d = frexp(x, &values[i++].i); p++;
  j = i++;
  values[j].desc = "frexpf";
  values[j].f = frexpf(xf, &values[i++].i); p++;

  values[i].desc = "ilogb";
  values[i++].i = ilogb(x); p++;
  values[i].desc = "ilogbf";
  values[i++].i = ilogbf(xf); p++;

  values[i].desc = "ldexp";
  values[i++].d = ldexp(x, n); p+=3;
  values[i].desc = "ldexpf";
  values[i++].f = ldexpf(xf, n); p+=3;

  call1(log);
  call1(log10);
  call1(log1p);
  call1(log2);
  call1(logb);

  j = i++;
  values[j].desc = "modf";
  values[j].d = modf(x, &values[i++].d); p++;
  j = i++;
  values[j].desc = "modff";
  values[j].f = modff(xf, &values[i++].f); p++;

  values[i].desc = "scalbn";
  values[i++].d = scalbn(x, n); p+=3;
  values[i].desc = "scalbnf";
  values[i++].f = scalbnf(xf, n); p+=3;

  values[i].desc = "scalbln";
  values[i++].d = scalbln(x, nl); p+=3;
  values[i].desc = "scalblnf";
  values[i++].f = scalblnf(xf, nl); p+=3;

  /* power/abs */

  call1(cbrt);
  call1(fabs);
  call2(hypot);
  call2(pow);
  call1(sqrt);

  /* error/gamma */
  call1(erf);
  call1(erfc);
  call1(lgamma);
  call1(tgamma);
  call1(ceil);
  call1(floor);
  call1(nearbyint);
  call1(rint);
  call1(lrint);

  values[i].desc = "lrint";
  values[i++].l = lrint(x); p++;
  values[i].desc = "lrintf";
  values[i++].l = lrintf(xf); p++;
  values[i].desc = "llrint";
  values[i++].ll = llrint(x); p++;
  values[i].desc = "llrintf";
  values[i++].ll = llrintf(xf); p++;

  call1(round);

  values[i].desc = "lround";
  values[i++].l = lround(x); p++;
  values[i].desc = "lroundf";
  values[i++].l = lroundf(xf); p++;
  values[i].desc = "llround";
  values[i++].ll = llround(x); p++;
  values[i].desc = "llroundf";
  values[i++].ll = llroundf(xf); p++;

  call1(trunc);

  /* rem/mod */
  call2(fmod);
  call2(remainder);

  j = i++;
  values[j].desc = "remquo";
  values[j].d = remquo(x, y, &values[i++].i); p+=2;
  j = i++;
  values[j].desc = "remquof";
  values[j].f = remquof(xf, yf, &values[i++].i); p+=2;

  call2(copysign);

  /* nan */

  values[i].desc = "nan";
  values[i++].d = nan("");
  values[i].desc = "nanf";
  values[i++].f = nanf("");

  call2(nextafter);

  /* min/max/dim */
  call2(fdim);
  call2(fmax);
  call2(fmin);
  values[i].desc = "fma";
  values[i++].d = fma(x, y, values[p+2].d); p+=3;
  values[i].desc = "fmaf";
  values[i++].d = fmaf(xf, yf, values[p+2].f); p+=3;

  /* comp */
  call2i(isgreater);
  call2i(isgreaterequal);
  call2i(isless);
  call2i(islessequal);
  call2i(islessgreater);
  call2i(isunordered);

#undef x
#undef xf
#undef y
#undef n
}
sl_enddef

// XIGNORE: ppp*:D

slr_decl(slr_var(double, x),
	 slr_var(long, n));

sl_def(t_main, void)
{
  int j;
  double x = .42;
  int n = 42;

  if (slr_len(x)) x = slr_get(x)[0];
  if (slr_len(n)) n = slr_get(n)[0];

  for(j = 0; j < MAX_TESTS; ++j) {
    values[j].f = values[j].d = x;
    values[j].i = values[j].l = values[j].ll = n;
  }

  sl_create(,,,,,,, do_test);
  sl_sync();

  double_t xd = x;
  float_t xf = x;
  output_float(xd, 1, 4);
  output_char('\n', 1);
  output_float(xf, 1, 4);
  output_char('\n', 1);
  output_float(HUGE_VAL, 1, 4);
  output_char('\n', 1);
  output_float(HUGE_VALF, 1, 4);
  output_char('\n', 1);
  output_float(INFINITY, 1, 4);
  output_char('\n', 1);
  output_float(NAN, 1, 4);
  output_char('\n', 1);
  output_int(FP_ILOGB0, 1);
  output_char('\n', 1);
  output_int(FP_ILOGBNAN, 1);
  output_char('\n', 1);
  output_float(M_E, 1, 4);
  output_char('\n', 1);
  output_float(M_LOG2E, 1, 4);
  output_char('\n', 1);
  output_float(M_LOG10E, 1, 4);
  output_char('\n', 1);
  output_float(M_LN2, 1, 4);
  output_char('\n', 1);
  output_float(M_LN10, 1, 4);
  output_char('\n', 1);
  output_float(M_PI, 1, 4);
  output_char('\n', 1);
  output_float(M_PI_2, 1, 4);
  output_char('\n', 1);
  output_float(M_PI_4, 1, 4);
  output_char('\n', 1);
  output_float(M_1_PI, 1, 4);
  output_char('\n', 1);
  output_float(M_2_PI, 1, 4);
  output_char('\n', 1);
  output_float(M_2_SQRTPI, 1, 4);
  output_char('\n', 1);
  output_float(M_SQRT2, 1, 4);
  output_char('\n', 1);
  output_float(M_SQRT1_2, 1, 4);
  output_char('\n', 1);

  for (j = 0; j < i; ++j)
    {
      const char *d = 	   values[j].desc ? values[j].desc : ".";
      output_string(d, 1); output_char(' ', 1);
      output_float(values[j].d, 1, 4); output_char(' ', 1);
      output_float(values[j].f, 1, 4); output_char(' ', 1);
      output_int(values[j].i, 1); output_char(' ', 1);
      output_int(values[j].l, 1); output_char(' ', 1);
      output_int(values[j].ll, 1); output_char('\n', 1);
    }
}
sl_enddef
