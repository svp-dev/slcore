//
// io.sl: this file is part of the slc project.
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
// $Id$
//

m4_include(svp/lib.slh)
m4_include(svp/div.slh)
m4_include(svp/io.slh)

static const char *digits = "0123456789abcdef";

sl_def(svp_io_putc, void,
       sl_glparm(char, c))
{
  __write1(sl_getp(c));
}
sl_enddef

sl_def(svp_io_write, void,
       sl_glparm(void*, gptr),
       sl_glparm(unsigned, gsz))
{
  const char *p = (const char*)(void*)sl_getp(gptr);
  unsigned i = 0, sz = sl_getp(gsz);
  while (likely(i < sz))
    __write1(p[i++]);
}
sl_enddef

sl_def(svp_io_puts, void,
       sl_glparm(const char *, gstr))
{
  const char *str = sl_getp(gstr);
  char c;
  while (likely(c = *str++))
    __write1(c);
}
sl_enddef

#define _puts(S) do { const char *p = (S); while(*p) __write1(*p++); } while(0)

sl_def(svp_io_putf, void,
       sl_glfparm(double, gx),
       sl_glparm(unsigned, gprec),
       sl_glparm(unsigned, gbase))
{
  double x = sl_getp(gx);
  unsigned prec = sl_getp(gprec);
  const unsigned base = sl_getp(gbase);

  if (unlikely(x != x)) _puts("nan");
  else if (unlikely(x == 1e5000)) _puts("+inf");
  else if (unlikely(x == -1e5000)) _puts("-inf");
  else {
      /* -- print the mantissa -- */
      if (x < 0.) { __write1('-'); x = -x; } else __write1('+');

      /* -- find exponent and normalize -- */
      int exp = 0;
      while (x >= base) { x /= base; ++exp; }
      while (x && x < 1.0) { x *= base; --exp; }

      unsigned d = (unsigned)x;
      __write1(digits[d]);
      __write1('.');

      x = (x - d) * base;
      while(--prec) {
	d = (unsigned)x;
	__write1(digits[d]);
	x = (x - d) * base;
      }

      /* -- print the exponent -- */
      __write1('E');
      if (exp < 0) { __write1('-'); exp = -exp; } else __write1('+');
      unsigned uexp = exp;
      if (uexp < base)
	__write1(digits[uexp]);
      else {
	unsigned rs = base;
	divmodu(uexp, rs);
	__write1(digits[rs]);
	__write1(digits[uexp]);
      }
    }
}
sl_enddef

sl_def(svp_io_putun, void,
       sl_glparm(unsigned long long, gn),
       sl_glparm(unsigned, gbase))
{
  unsigned long long x = sl_getp(gn);
  const unsigned base = sl_getp(gbase);
  if (x < base) __write1(digits[x]);
  else {
      unsigned long long root = 1;
      while (divu(x, root) >= base)
	root *= base;
      while (root) {
	unsigned long long rs = root;
	divmodu(x, rs);
	__write1(digits[rs]);
	rs = base;
	divmodu(root, rs);
	root = rs;
      }
    }
}
sl_enddef

sl_def(svp_io_putn, void,
       sl_glparm(long long, gn),
       sl_glparm(unsigned, gbase))
{
  long long x = sl_getp(gn);
  const unsigned base = sl_getp(gbase);
  if (!x) __write1('0');
  else {
    long long root;
    if (x < 0) {
      root = -1;
      __write1('-');
    } else root = 1;
    while (divs(x, root) >= base)
      root *= base;
    while (root) {
      long long rs = root;
      divmods(x, rs);
      __write1(digits[rs]);
      rs = (long long)base;
      divmods(root, rs);
      root = rs;
    }
  }
}
sl_enddef

sl_def(svp_io_printf, void,
       sl_glparm(const char*, gfmt),
       sl_glparm(const uint8_t*, gsz),
       sl_glparm(svp_arg_slot*, gdata))
{
  const char *fmt;
  const svp_arg_slot *data;
  const uint8_t *sz;

  for (fmt = sl_getp(gfmt),
       data = sl_getp(gdata),
       sz = sl_getp(gsz); *fmt; ++fmt) {

    if (likely(*fmt != '%'))
      __write1(*fmt);
    else {
      long long sdata;
      unsigned long long udata;
      double fdata;
      char cfmt = *++fmt;

      switch(cfmt) {
      case 'c':
      case 'u':
      case 'd':
      case 'x':
	switch(*sz++) {
	case 8: udata = data->uq; sdata = (data++)->sq; break;
	case 4: udata = data->ul; sdata = (data++)->sl; break;
	case 2: udata = data->us; sdata = (data++)->ss; break;
	case 1: udata = data->ub; sdata = (data++)->sb; break;
	default: __write1('?'); udata = 0; sdata = 0; ++data; break;
	}
	break;
      case 'f':
      case 'g':
	switch(*sz++) {
	case sizeof(double): fdata = (data++)->d; break;
	default: fdata = (data++)->f; break;
	}
	break;
      case 's':
	++sz;
      }

      switch(cfmt) {
      case 'c': sl_proccall(svp_io_putc,
			    sl_glarg(char, c, (char)sdata));
	break;
      case 's': sl_proccall(svp_io_puts,
			    sl_glarg(const char*, p, (const char*)(data++)->ptr));
	break;
      case 'f': sl_proccall(svp_io_putf,
			    sl_glfarg(double, d, fdata),
			    sl_glarg(unsigned, prec, 7),
			    sl_glarg(unsigned, base, 10));
	break;
      case 'g': sl_proccall(svp_io_putf,
			    sl_glfarg(double, d, fdata),
			    sl_glarg(unsigned, prec, 15),
			    sl_glarg(unsigned, base, 10));
	break;
      case 'd': sl_proccall(svp_io_putn,
			    sl_glarg(long long, n, sdata),
			    sl_glarg(unsigned, base, 10));
	break;
      case 'u': sl_proccall(svp_io_putun,
			    sl_glarg(unsigned long long, n, udata),
			    sl_glarg(unsigned, base, 10));
	break;
      case 'x': sl_proccall(svp_io_putun,
			    sl_glarg(unsigned long long, n, udata),
			    sl_glarg(unsigned, base, 16));
	break;
      default: __write1('%'); __write1(*fmt); break;
      }
    }
  }
}

sl_enddef
