//
// io.c: this file is part of the SL toolchain.
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

#include <svp/compiler.h>
#include <svp/testoutput.h>
#include <math.h>

#define __shutup_io_h
#include <svp/deprecated/div.h>
#include <svp/deprecated/io.h>

static const char digits[] = "0123456789abcdef";

sl_def(svp_io_putc, void,
       sl_glparm(char, c))
{
  output_char(sl_getp(c), 1);
}
sl_enddef

sl_def(svp_io_write, void,
       sl_glparm(void*, gptr),
       sl_glparm(size_t, gsz))
{
  output_bytes(sl_getp(gptr), sl_getp(gsz), 1);
}
sl_enddef

sl_def(svp_io_puts, void,
       sl_glparm(const char *, gstr))
{
  output_string(sl_getp(gstr), 1);
}
sl_enddef

sl_def(svp_io_putf, void,
       sl_glfparm(double, gx),
       sl_glparm(unsigned, gprec),
       sl_glparm(unsigned, gbase))
{
  double x = sl_getp(gx);
  unsigned prec = sl_getp(gprec);
  const unsigned long base = sl_getp(gbase);

  if (unlikely(isnan(x))) output_string("nan", 1);
  else if (unlikely(isinf(x))) output_string(signbit(x) ? "-inf" : "+inf", 1);
  else {
      /* -- print the mantissa -- */
    if (x < 0.) { output_char('-', 1); x = -x; } else output_char('+', 1);

      /* -- find exponent and normalize -- */
      int exp = 0;
      while (x >= base) { x /= base; ++exp; }
      while (x && x < 1.0) { x *= base; --exp; }

      unsigned long d = (unsigned long)x;
      output_char(digits[d], 1);
      output_char('.', 1);

      x = (x - d) * base;
      while(--prec) {
	d = (unsigned long)x;
	output_char(digits[d], 1);
	x = (x - d) * base;
      }

      /* -- print the exponent -- */
      output_char('E', 1);
      if (exp < 0) { output_char('-', 1); exp = -exp; } else output_char('+', 1);
      unsigned long uexp = exp;
      sl_proccall(svp_io_putun, sl_glarg(unsigned long, gn, uexp), sl_glarg(unsigned, gbase, base));
    }
}
sl_enddef

sl_def(svp_io_putun, void,
       sl_glparm(unsigned long, gn),
       sl_glparm(unsigned, gbase))
{
  unsigned long x = sl_getp(gn);
  const unsigned long base = sl_getp(gbase);
  if (x < base) output_char(digits[x], 1);
  else {
      unsigned long root = 1;
      while (divu(x, root) >= base)
	root *= base;
      while (root) {
	unsigned long rs = root;
	divmodu(x, rs);
	output_char(digits[rs], 1);
	rs = base;
	divmodu(root, rs);
	root = rs;
      }
    }
}
sl_enddef

sl_def(svp_io_putn, void,
       sl_glparm(long, gn),
       sl_glparm(unsigned, gbase))
{
  long x = sl_getp(gn);
  const unsigned long base = sl_getp(gbase);
  if (!x) output_char('0', 1);
  else {
    long root;
    if (x < 0) {
      root = -1;
      output_char('-', 1);
    } else root = 1;
    while (divs(x, root) >= base)
      root *= base;
    while (root) {
      long rs = root;
      divmods(x, rs);
      output_char(digits[rs], 1);
      rs = (long)base;
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
      output_char(*fmt, 1);
    else {
      long sdata;
      unsigned long udata;
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
	default: output_char('?', 1); udata = 0; sdata = 0; ++data; break;
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
			    sl_glarg(long, n, sdata),
			    sl_glarg(unsigned, base, 10));
	break;
      case 'u': sl_proccall(svp_io_putun,
			    sl_glarg(unsigned long, n, udata),
			    sl_glarg(unsigned, base, 10));
	break;
      case 'x': sl_proccall(svp_io_putun,
			    sl_glarg(unsigned long, n, udata),
			    sl_glarg(unsigned, base, 16));
	break;
      default: output_char('%', 1); output_char(*fmt, 1); break;
      }
    }
  }
}

sl_enddef
