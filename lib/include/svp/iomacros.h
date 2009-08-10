//
// iomacros.slh: this file is part of the slc project.
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
#ifndef __SVP_IOMACROS_H__
# define __SVP_IOMACROS_H__

#include <svp/io.h>

/*
 * The following macro definitions encapsulate the corresponding
 * thread functions from svp/io.slh as macros that are suitable
 * for use as statements in programs.
 */

#define putc(C) sl_proccall(svp_io_putc, sl_glarg(char, __gc, (C)))

#define puts(S) sl_proccall(svp_io_puts, sl_glarg(const char*, __gs, (S)))

#define write(Buf, Len)	 sl_proccall(svp_io_write,			\
				     sl_glarg(void*, __gs, (Buf)),	\
				     sl_glarg(size_t, __gl, (Len)))

#define putf(N, Prec) sl_proccall(svp_io_putf,				\
				  sl_glfarg(double, __gn, (N)),		\
				  sl_glarg(unsigned, __gp, (Prec)),	\
				  sl_glarg(unsigned, __gb, 10))

#define putn(N) sl_proccall(svp_io_putn,			\
			    sl_glarg(long, __gn, (N)),	\
			    sl_glarg(unsigned, __gb, 10))

#define putu(N) sl_proccall(svp_io_putun,				\
			    sl_glarg(unsigned long, __gn, (N)),	\
			    sl_glarg(unsigned, __gb, 10))

/* the following M4 code tries very hard to simulate printf's vararg
   syntax without actual support for varargs in the compiler. */

m4_define([[printf]],
[[do {
  static const uint8_t __pf_sz[m4_count(m4_shift($@))] = {
    m4_if([[$2]],[[]],,[[
      m4_foreach([[printf_arg]],m4_dquote(m4_shift($@)),[[sizeof(printf_arg),]])
    ]])
  };
  volatile struct {
    m4_if([[$2]],[[]],,[[
      m4_pushdef([[printf_argcount]],0)
      m4_foreach([[printf_arg]],m4_dquote(m4_shift($@)),[[
        union { __typeof__(printf_arg) __arg; svp_arg_slot __slot; }
          __a[[]]printf_argcount;
        m4_step([[printf_argcount]])
      ]])
      m4_popdef([[printf_argcount]])
    ]])
  } __pf_args;
  m4_if([[$2]],[[]],,[[
   m4_pushdef([[printf_argcount]],0)
   m4_foreach([[printf_arg]],m4_dquote(m4_shift($@)),[[
   __pf_args.__a[[]]printf_argcount[[]].__arg = (printf_arg);
   m4_step([[printf_argcount]])
   ]])
   m4_popdef([[printf_argcount]])
  ]])
  sl_proccall(svp_io_printf,
              sl_glarg(const char*, gfmt, ([[$1]])),
	      sl_glarg(const uint8_t*, gsz, (__pf_sz)),
              sl_glarg(svp_arg_slot*, gdata, (svp_arg_slot*)(void*)&__pf_args));
} while(0)]])


#endif
