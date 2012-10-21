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

/* Definition for a memory barrier primitive */

#if defined(__GNUC__) && !defined(__AVOID_GNUISMS)
# define barrier() __asm__ __volatile__("#MB" : : : "memory")
#else
# ifdef __INTEL_COMPILER
#  define barrier() __memory_barrier()
# else
#  warning "No definition for barrier()"
#  define barrier() /* nothing */
# endif
#endif

/* Branch optimization */

#if defined(__GNUC__) && !defined(__AVOID_GNUISMS)
# define likely(X) __builtin_expect(!!(X), 1)
# define unlikely(X) __builtin_expect(!!(X), 0)
#else
# define likely(X) (X)
# define unlikely(X) (X)
#endif

/* No operation instruction */

#if defined(__GNUC__) && !defined(__AVOID_GNUISMS)
# if defined(__mt_freestanding__) && !defined(__mips__)
#  define nop() __asm__ __volatile__("nop\n\tswch")
# else
#  define nop() __asm__ __volatile__("nop")
# endif
#else
# define nop() do { } while(0)
#endif

/* inlining words */

#if defined (__GNUC__) && !defined(__AVOID_GNUISMS)
# define noinline __attribute__((__noinline__))
# define alwaysinline __attribute__((__always_inline__)) static
# define unused __attribute__((__unused__))
#else
# warning "No definition for noinline/alwaysinline on this target."
# define noinline /* nothing */
# define alwaysinline static inline
# define unused /* nothing */
#endif

#if defined(__mt_freestanding__) && defined(__GNUC__) && !defined(__AVOID_GNUISMS)
# define use(X) __asm__ __volatile__("#USE %0" : "=rf"(X) : "0"(X))
#else
# define use(X) do { (X) = (X); } while(0)
#endif

#endif // ! SLC_SVP_COMPILER_H
