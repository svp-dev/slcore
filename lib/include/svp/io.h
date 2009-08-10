//
// io.slh: this file is part of the slc project.
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
#ifndef __SVP_IO_H__
# define __SVP_IO_H__

#include <svp/argslot.h>

/* Print a single character to the console output. */
sl_decl(svp_io_putc, void,
	sl_glparm(char, c));

/* Print a nul-terminated string to the console output. */
sl_decl(svp_io_puts, void,
	sl_glparm(const char *, gstr));

/* Print an arbitrary number of bytes to the console output. */
sl_decl(svp_io_write, void,
	sl_glparm(void *, gptr),
	sl_glparm(unsigned, gsize));

/* Print a floating point number to the console output. */
/* Note: the floating point is normalized before printing. */
sl_decl(svp_io_putf, void,
	sl_glfparm(double, gx),
	sl_glparm(unsigned, gprec),
	sl_glparm(unsigned, gbase));

/* Print an unsigned integer to the console output. */
sl_decl(svp_io_putun, void,
	sl_glparm(unsigned long long, gn),
	sl_glparm(unsigned, gbase));

/* Print a nul-terminated string to the console output. */
sl_decl(svp_io_puts, void,
	sl_glparm(const char *, gstr));

/* Print a floating point number to the console output. */
/* Note: the floating point is normalized before printing. */
sl_decl(svp_io_putf, void,
	sl_glfparm(double, gx),
	sl_glparm(unsigned, gprec),
	sl_glparm(unsigned, gbase));

/* Print a signed integer to the console output. */
sl_decl(svp_io_putn, void,
	sl_glparm(long long, gn),
	sl_glparm(unsigned, gbase));

/* Format and print arguments to the console output. */
/* Recognized formats:
   %c - single character
   %s - nul-terminated string
   %f - floating point number in base 10 with 7 digits of precision
   %g - floating point number in base 10 with 15 digits of precision
   %d - signed integer in base 10
   %u - unsigned integer in base 10
   %x - unsigned integer in base 16
   %% - the character % itself.
*/
sl_decl(svp_io_printf, void,
	sl_glparm(const char*, gfmt),
	sl_glparm(const unsigned char*, gsz),
	sl_glparm(svp_arg_slot*, gdata));

#endif
