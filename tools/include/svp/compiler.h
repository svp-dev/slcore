//
// compiler.h: this file is part of the SL toolchain.
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

#ifndef SLC_SVP_COMPILER_H
# define SLC_SVP_COMPILER_H

#ifndef __GNUC__
# warn __GNUC__ is not defined. Some macros here may not work.
#endif

/* The following code is inspired from <linux/compiler.h> */

/* Definition for a memory barrier primitive */
#define barrier() __asm__ __volatile__("" : : : "memory")

#ifdef __INTEL_COMPILER
# undef barrier
# define barrier() __memory_barrier()
#endif

/* Branch optimization */

#define likely(X) __builtin_expect(!!(X), 1)
#define unlikely(X) __builtin_expect(!!(X), 0)

/* No operation instruction */

#ifdef __mt_freestanding__
# define nop() __asm__ __volatile__("nop\n\tswch");
#else
# define nop() __asm__ __volatile__("nop");
#endif

#endif // ! SLC_SVP_COMPILER_H
