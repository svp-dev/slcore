// utcdebug.h: this file is part of the utcc project.
//
// Copyright (C) 2008 The CSA group.
//
// $Id$
//
#ifndef UTC_UTCDEBUG_H
# define UTC_UTCDEBUG_H

#include "debugimpl.h"

#define __gen_div(divmod, T, X, Y) \
  ({ \
    T __gen_div_x = (X), __gen_div_y = (Y); \
    divmod(__gen_div_x, __gen_div_y); \
    __gen_div_y; })
#define __gen_mod(divmod, T, X, Y) \
  ({ \
    T __gen_div_x = (X), __gen_div_y = (Y);\
    divmod(__gen_div_x, __gen_div_y); \
    __x;		      \
  })
#define d_divu32(X, Y) __gen_div(d_divmodu32, X, Y)
#define d_divs32(X, Y) __gen_div(d_divmods32, X, Y)
#define d_divu64(X, Y) __gen_div(d_divmodu64, X, Y)
#define d_divs64(X, Y) __gen_div(d_divmods64, X, Y)

#define d_puts(Message) do {          \
  const char *__s = (Message);        \
  while (*__s) d_putchar(*__s++);     \
} while(0)

#ifndef NDEBUG
#define d_assert(e) \
   ((void) ((e) ? 0 : d__assert (#e, __FILE__, __LINE__)))
#define d__assert(e, file, line) d___assert(e, file, line)
#define d___assert(e, file, line) \
  ({d_puts(file ":" #line ": failed assertion '" e "\n"); d_abort();})
#else /* NDEBUG */
#define d_assert(ignore) ((void)0)
#endif

static const char *__basechars = "0123456789abcdef";

#define d_dots(X) do {		  \
    unsigned long long __x = (X);	  \
    while (__x--) d_putchar('.'); \
    d_putchar('\n');		  \
  } while(0)

#define d_put_unsigned(X, divmod, T, Base) do {				\
    unsigned T __x = (unsigned T)(X);					\
    if (__x < Base) d_putchar(__basechars[__x]);			\
    else {								\
      unsigned T __root = 1;						\
      while (__gen_div(divmod, unsigned T, __x, __root) >= Base)	\
	__root *= Base;							\
      while (__root) {							\
	unsigned T __rs = __root;					\
	divmod(__x, __rs);						\
	d_putchar(__basechars[__rs]);					\
	__rs = Base;							\
	divmod(__root, __rs);						\
	__root = __rs;							\
      }									\
    }									\
  } while(0)

#define d_put_signed(X, divmod, T, Base) do {	      \
    signed T __x = (signed T)(X);		      \
    if (!__x) d_putchar('0');			      \
    else {					      \
      signed T __root;				      \
      if (__x < 0) {				      \
	__root = -1;				      \
	d_putchar('-');					\
      } else __root = 1;					 \
      while (__gen_div(divmod, signed T, __x, __root) >= Base) \
	__root *= Base;						 \
      while (__root) {						 \
	signed T __rs = __root;					 \
	divmod(__x, __rs);					 \
	d_putchar(__basechars[__rs]);				 \
	__rs = Base;						 \
	divmod(__root, __rs);					 \
	__root = __rs;						 \
      }								 \
    }								 \
  } while(0)

#define d_put_float(X, T, Prec, Base) do {    \
    T __x = (T)(X);				      \
    if (__x != __x) d_puts("nan");		      \
    else if (__x == 1e5000) d_puts("+inf");	      \
    else if (__x == -1e5000) d_puts("-inf");	      \
    else {					      \
      /* -- find exponent and normalize -- */	      \
      int __e = 0;				      \
      while (__x >= Base) { __x /= Base; ++__e; } \
      while (__x < 1.0) { __x *= Base; --__e; }     \
      /* -- print the mantissa -- */		      \
      if (__x < 0.) { d_putchar('-'); __x = -__x; }   \
      unsigned __d = (unsigned)__x;		      \
      d_putchar(__basechars[__d]);		      \
      d_putchar('.');				      \
      __x = (__x - __d) * Base;		      \
      unsigned __prec = (Prec);			      \
      while(--__prec) {				      \
	__d = (unsigned)__x;			      \
	d_putchar(__basechars[__d]);		      \
	__x = (__x - __d) * Base;		      \
      }						      \
      /* -- print the exponent -- */		      \
      d_putchar('E');				      \
      if (__e < 0) { d_putchar('-'); __e = -__e; }    \
      if (__e < (signed)Base)			      \
	d_putchar(__basechars[__e]);		      \
      else {					      \
	unsigned __rs = Base;			      \
	d_divmodu32(__e, __rs);			      \
	d_putchar(__basechars[__rs]);		      \
	d_putchar(__basechars[__e]);		      \
      }						      \
    }						      \
  } while(0)

#define d_put_ui32d(X) d_put_unsigned(X, d_divmodu32, int, 10)
#define d_put_ui32o(X) d_put_unsigned(X, d_divmodu32, int, 8)
#define d_put_ui32x(X) d_put_unsigned(X, d_divmodu32, int, 16)
#define d_put_si32d(X) d_put_signed(X, d_divmods32, int, 10)
#define d_put_si32o(X) d_put_signed(X, d_divmods32, int, 8)
#define d_put_si32x(X) d_put_signed(X, d_divmods32, int, 16)

#define d_put_ui64d(X) d_put_unsigned(X, d_divmodu64, long long, 10)
#define d_put_ui64o(X) d_put_unsigned(X, d_divmodu64, long long, 8)
#define d_put_ui64x(X) d_put_unsigned(X, d_divmodu64, long long, 16)
#define d_put_si64d(X) d_put_signed(X, d_divmods64, long long, 10)
#define d_put_si64o(X) d_put_signed(X, d_divmods64, long long, 8)
#define d_put_si64x(X) d_put_signed(X, d_divmods64, long long, 16)


#if 0
#define EMIT(X)							\
  if (*__fmt)							\
    switch(*(__fmt+1)) {					\
    case 'c': d_putchar((char)(X)); break;			\
    case 's': d_puts((const char*)(X)); break;	\
    case 'f': d_put_float(X, float, 7, 10); break;		\
    case 'g': d_put_float(X, double, 15, 10); break;		\
    case 'd': case 'i': d_put_signed(X, d_divmods32, int, 10); break;	\
    case 'u': d_put_unsigned(X, d_divmodu32, int, 10); break;	\
    case 'x': d_put_unsigned(X, d_divmodu32, int, 16); break;	\
    }; __fmt += 2						\

#define ADVANCE                                               \
  while (*__fmt && (*__fmt != '%' || *(__fmt+1) == '%')) {    \
    d_putchar((*__fmt == '%') ? (++__fmt, '%') : *__fmt);     \
    ++__fmt;						      \
  }							      \

#define d_printf1(Fmt, X) do {                                \
    const char *__fmt = (Fmt);				      \
    ADVANCE; EMIT(X); ADVANCE;				      \
  } while(0)

#define d_printf2(Fmt, X, Y) do {                             \
    const char *__fmt = (Fmt);				      \
    ADVANCE; EMIT(X); ADVANCE; EMIT(Y); ADVANCE;	      \
  } while(0)

#define d_printf3(Fmt, X, Y, Z) do {                          \
    const char *__fmt = (Fmt);				      \
    ADVANCE; EMIT(X); ADVANCE; EMIT(Y);			      \
    ADVANCE; EMIT(Z); ADVANCE;				      \
  } while(0)

#define d_printf4(Fmt, X, Y, Z, T) do {                       \
    const char *__fmt = (Fmt);				      \
    ADVANCE; EMIT(X); ADVANCE; EMIT(Y);			      \
    ADVANCE; EMIT(Z); ADVANCE; EMIT(T); ADVANCE;	      \
  } while(0)
#endif

#define d_force_use(X) __asm__ volatile("/* seen here: " #X "*/" : : "r"(X))

#endif // ! UTC_UTCDEBUG_H
