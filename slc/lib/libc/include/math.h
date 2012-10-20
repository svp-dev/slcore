//
// math.h: this file is part of the SL toolchain.
//
// Copyright (C) 2009,2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#ifndef SLC_MTA_MATH_H
# define SLC_MTA_MATH_H

#if __FLT_EVAL_METHOD__ == 0
typedef float float_t;
typedef double double_t;
#elif __FLT_EVAL_METHOD__ == 1
typedef double float_t;
typedef double double_t;
#elif __FLT_EVAL_METHOD__ == 2 || __FLT_EVAL_METHOD__ == -1
typedef long double float_t;
typedef long double double_t;
#else /* __FLT_EVAL_METHOD__ */
        #error "Unsupported value of __FLT_EVAL_METHOD__."
#endif /* __FLT_EVAL_METHOD__ */


# define HUGE_VAL (__builtin_huge_val())
# define HUGE_VALF (__builtin_huge_valf())
# define INFINITY (__builtin_inff())
# define NAN (__builtin_nanf(""))

/* The values returned by `ilogb' for 0 and NaN respectively.  */
# define FP_ILOGB0     (-2147483647)
# define FP_ILOGBNAN   (2147483647)

# define MATH_ERRNO     1       /* errno set by math functions.  */
# define MATH_ERREXCEPT 2       /* Exceptions raised by math functions.  */
# define math_errhandling 0

# define M_E            2.7182818284590452354   /* e */
# define M_LOG2E        1.4426950408889634074   /* log_2 e */
# define M_LOG10E       0.43429448190325182765  /* log_10 e */
# define M_LN2          0.69314718055994530942  /* log_e 2 */
# define M_LN10         2.30258509299404568402  /* log_e 10 */
# define M_PI           3.14159265358979323846  /* pi */
# define M_PI_2         1.57079632679489661923  /* pi/2 */
# define M_PI_4         0.78539816339744830962  /* pi/4 */
# define M_1_PI         0.31830988618379067154  /* 1/pi */
# define M_2_PI         0.63661977236758134308  /* 2/pi */
# define M_2_SQRTPI     1.12837916709551257390  /* 2/sqrt(pi) */
# define M_SQRT2        1.41421356237309504880  /* sqrt(2) */
# define M_SQRT1_2      0.70710678118654752440  /* 1/sqrt(2) */

/* constants from uclibc */
enum
  {
    FP_NAN = 0,
# define FP_NAN FP_NAN
    FP_INFINITE = 1,
# define FP_INFINITE FP_INFINITE
    FP_ZERO = 2,
# define FP_ZERO FP_ZERO
    FP_SUBNORMAL = 3,
# define FP_SUBNORMAL FP_SUBNORMAL
    FP_NORMAL = 4
# define FP_NORMAL FP_NORMAL
  };

#define fpclassify(x) __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, (x))
#define signbit(x) __builtin_signbit(x)
#define isfinite(x) __builtin_isfinite(x)
#define isnormal(x) __builtin_isnormal(x)
#define isnan(x) __builtin_isnan(x)
#define isinf(x) __builtin_isinf(x)

/* 7.12.4 trigonometric functions */

double acos(double x);
float acosf(float x);
#define acos(x) __builtin_acos(x)
#define acosf(x) __builtin_acosf(x)

double asin(double x);
float asinf(float x);
#define asin(x) __builtin_asin(x)
#define asinf(x) __builtin_asinf(x)

double atan(double x);
float atanf(float x);
#define atan(x) __builtin_atan(x)
#define atanf(x) __builtin_atanf(x)

double atan2(double y, double x);
float atan2f(float y, float x);
#define atan2(x, y) __builtin_atan2(x, y)
#define atan2f(x, y) __builtin_atan2f(x, y)

double cos(double x);
float cosf(float x);
#define cos(x) __builtin_cos(x)
#define cosf(x) __builtin_cosf(x)

double sin(double x);
float sinf(float x);
#define sin(x) __builtin_sin(x)
#define sinf(x) __builtin_sinf(x)

double tan(double x);
float tanf(float x);
#define tan(x) __builtin_tan(x)
#define tanf(x) __builtin_tanf(x)

/* 7.12.5 hyperbolic functions */

double acosh(double x);
float acoshf(float x);
#define acosh(x) __builtin_acosh(x)
#define acoshf(x) __builtin_acoshf(x)

double asinh(double x);
float asinhf(float x);
#define asinh(x) __builtin_asinh(x)
#define asinhf(x) __builtin_asinhf(x)

double atanh(double x);
float atanhf(float x);
#define atanh(x) __builtin_atanh(x)
#define atanhf(x) __builtin_atanhf(x)

double cosh(double x);
float coshf(float x);
#define cosh(x) __builtin_cosh(x)
#define coshf(x) __builtin_coshf(x)

double sinh(double x);
float sinhf(float x);
#define sinh(x) __builtin_sinh(x)
#define sinhf(x) __builtin_sinhf(x)

double tanh(double x);
float tanhf(float x);
#define tanh(x) __builtin_tanh(x)
#define tanhf(x) __builtin_tanhf(x)

/* 7.12.6 Exponential and logarithmic */

double exp(double x);
float expf(float x);
#define exp(x) __builtin_exp(x)
#define expf(x) __builtin_expf(x)

double exp2(double x);
float exp2f(float x);
#define exp2(x) __builtin_exp2(x)
#define exp2f(x) __builtin_exp2f(x)

double expm1(double x);
float expm1f(float x);
#define expm1(x) __builtin_expm1(x)
#define expm1f(x) __builtin_expm1f(x)

double frexp(double value, int *exp);
float frexpf(float value, int *exp);
#define frexp(x, y) __builtin_frexp(x, y)
#define frexpf(x, y) __builtin_frexpf(x, y)

int ilogb(double x);
int ilogbf(float x);
#define ilogb(x) __builtin_ilogb(x)
#define ilogbf(x) __builtin_ilogbf(x)

double ldexp(double x, int exp);
float ldexpf(float x, int exp);
#define ldexp(x, y) __builtin_ldexp(x, y)
#define ldexpf(x, y) __builtin_ldexpf(x, y)

double log(double x);
float logf(float x);
#define log(x) __builtin_log(x)
#define logf(x) __builtin_logf(x)

double log10(double x);
float log10f(float x);
#define log10(x) __builtin_log10(x)
#define log10f(x) __builtin_log10f(x)

double log1p(double x);
float log1pf(float x);
#define log1p(x) __builtin_log1p(x)
#define log1pf(x) __builtin_log1pf(x)

double log2(double x);
float log2f(float x);
#define log2(x) __builtin_log2(x)
#define log2f(x) __builtin_log2f(x)

double logb(double x);
float logbf(float x);
#define logb(x) __builtin_logb(x)
#define logbf(x) __builtin_logbf(x)

double modf(double value, double *iptr);
float modff(float value, float *iptr);
#define modf(x, y) __builtin_modf(x, y)
#define modff(x, y) __builtin_modff(x, y)

double scalbn(double x, int n);
float scalbnf(float x, int n);
#define scalbn(x, y) __builtin_scalbn(x, y)
#define scalbnf(x, y) __builtin_scalbnf(x, y)

double scalbln(double x, long int n);
float scalblnf(float x, long int n);
#define scalbln(x, y) __builtin_scalbln(x, y)
#define scalblnf(x, y) __builtin_scalblnf(x, y)

/* 7.12.7 power and absolute value */

double cbrt(double x);
float cbrtf(float x);
#define cbrt(x) __builtin_cbrt(x)
#define cbrtf(x) __builtin_cbrtf(x)

double fabs(double x);
float fabsf(float x);
#define fabs(x) __builtin_fabs(x)
#define fabsf(x) __builtin_fabsf(x)

double hypot(double x, double y);
float hypotf(float x, float y);
#define hypot(x, y) __builtin_hypot(x, y)
#define hypotf(x, y) __builtin_hypotf(x, y)

double pow(double x, double y);
float powf(float x, float y);
#define pow(x, y) __builtin_pow(x, y)
#define powf(x, y) __builtin_powf(x, y)

double sqrt(double x);
float sqrtf(float x);
#define sqrt(x) __builtin_sqrt(x)
#define sqrtf(x) __builtin_sqrtf(x)

/* 7.12.8 Error and gamma functions */

double erf(double x);
float erff(float x);
#define erf(x) __builtin_erf(x)
#define erff(x) __builtin_erff(x)

double erfc(double x);
float erfcf(float x);
#define erfc(x) __builtin_erfc(x)
#define erfcf(x) __builtin_erfcf(x)

double lgamma(double x);
float lgammaf(float x);
#define lgamma(x) __builtin_lgamma(x)
#define lgammaf(x) __builtin_lgammaf(x)

extern int signgam;

double tgamma(double x);
float tgammaf(float x);
#define tgamma(x) __builtin_tgamma(x)
#define tgammaf(x) __builtin_tgammaf(x)

/* 7.12.9 nearest integer functions */

double ceil(double x);
float ceilf(float x);
#define ceil(x) __builtin_ceil(x)
#define ceilf(x) __builtin_ceilf(x)

double floor(double x);
float floorf(float x);
#define floor(x) __builtin_floor(x)
#define floorf(x) __builtin_floorf(x)

double nearbyint(double x);
float nearbyintf(float x);
#define nearbyint(x) __builtin_nearbyint(x)
#define nearbyintf(x) __builtin_nearbyintf(x)

double rint(double x);
float rintf(float x);
#define rint(x) __builtin_rint(x)
#define rintf(x) __builtin_rintf(x)

long int lrint(double x);
long int lrintf(float x);
#define lrint(x) __builtin_lrint(x)
#define lrintf(x) __builtin_lrintf(x)

long long int llrint(double x);
long long int llrintf(float x);
#define llrint(x) __builtin_llrint(x)
#define llrintf(x) __builtin_llrintf(x)

double round(double x);
float roundf(float x);
#define round(x) __builtin_round(x)
#define roundf(x) __builtin_roundf(x)

long int lround(double x);
long int lroundf(float x);
#define lround(x) __builtin_lround(x)
#define lroundf(x) __builtin_lroundf(x)

long long int llround(double x);
long long int llroundf(float x);
#define llround(x) __builtin_llround(x)
#define llroundf(x) __builtin_llroundf(x)

double trunc(double x);
float truncf(float x);
#define trunc(x) __builtin_trunc(x)
#define truncf(x) __builtin_truncf(x)

/* 7.12.10 remainder functions */

double fmod(double x, double y);
float fmodf(float x, float y);
#define fmod(x, y) __builtin_fmod(x, y)
#define fmodf(x, y) __builtin_fmodf(x, y)

double remainder(double x, double y);
float remainderf(float x, float y);
#define remainder(x, y) __builtin_remainder(x, y)
#define remainderf(x, y) __builtin_remainderf(x, y)

double remquo(double x, double y, int *quo);
float remquof(float x, float y, int *quo);
#define remquo(x, y, i) __builtin_remquo(x, y, i)
#define remquof(x, y, i) __builtin_remquof(x, y, i)

double copysign(double x, double y);
float copysignf(float x, float y);
#define copysign(x, y) __builtin_copysign(x, y)
#define copysignf(x, y) __builtin_copysignf(x, y)

/* 7.12.11 the nan functions */

double nan(const char *tagp);
float nanf(const char *tagp);
#define nan(x) __builtin_nan(x)
#define nanf(x) __builtin_nanf(x)

double nextafter(double x, double y);
float nextafterf(float x, float y);
#define nextafter(x, y) __builtin_nextafter(x, y)
#define nextafterf(x, y) __builtin_nextafterf(x, y)

/*** NOT SUPPORTED YET: ***
double nexttoward(double x, long double y);
float nexttowardf(float x, long double y);
#define nexttoward(x, y) __builtin_nexttoward(x, y)
#define nexttowardf(x, y) __builtin_nexttowardf(x, y)
*/

/* 7.12.12 max, min, and positive differences */

double fdim(double x, double y);
float fdimf(float x, float y);
#define fdim(x, y) __builtin_fdim(x, y)
#define fdimf(x, y) __builtin_fdimf(x, y)

double fmax(double x, double y);
float fmaxf(float x, float y);
#define fmax(x, y) __builtin_fmax(x, y)
#define fmaxf(x, y) __builtin_fmaxf(x, y)

double fmin(double x, double y);
float fminf(float x, float y);
#define fmin(x, y) __builtin_fmin(x, y)
#define fminf(x, y) __builtin_fminf(x, y)

/* 7.12.13 fp multiply-add */

double fma(double x, double y, double z);
float fmaf(float x, float y, float z);
#define fma(x, y, z) __builtin_fma(x, y, z)
#define fmaf(x, y, z) __builtin_fmaf(x, y, z)

/* 7.12.14 comparison macros */

# define isgreater(x, y)        __builtin_isgreater(x, y)
# define isgreaterequal(x, y)   __builtin_isgreaterequal(x, y)
# define isless(x, y)           __builtin_isless(x, y)
# define islessequal(x, y)      __builtin_islessequal(x, y)
# define islessgreater(x, y)    __builtin_islessgreater(x, y)
# define isunordered(u, v)      __builtin_isunordered(u, v)

#endif // ! SLC_MTA_MATH_H 
