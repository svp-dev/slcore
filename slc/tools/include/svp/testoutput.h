//
// testoutput.h: this file is part of the SL toolchain.
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

#ifndef SLC_SVP_TESTOUTPUT_H
# define SLC_SVP_TESTOUTPUT_H

#ifdef __mt_freestanding__

#include <svp/compiler.h>

# if defined(__alpha__) || defined(__mtalpha__)
#  define output_float(F, Stream, Precision) __asm__ __volatile("printf %0, %1" : : "f"(F), \
                                                                "r"((Stream) | (Precision << 4)))

#  define output_uint(N, Stream) __asm__ __volatile__("print %0, %1" : : "r"(N), "rI"((Stream) | (0 << 6)))
#  define output_hex(N, Stream) __asm__ __volatile__("print %0, %1" : : "r"(N), "rI"((Stream) | (1 << 6)))
#  define output_int(N, Stream) __asm__ __volatile__("print %0, %1" : : "r"(N), "rI"((Stream) | (2 << 6)))
#  define output_char(N, Stream) __asm__ __volatile__("print %0, %1" : : "r"(N), "rI"((Stream) | (3 << 6)))

# else
#  warning "Don't know how to define debug_int/debug_float on this platform."
# endif

# define output_string(S, Stream)                                       \
    do {								\
        const unsigned char *__ptr = (const unsigned char*)(void*)(S);  \
        while(likely(*__ptr)) output_char(*__ptr++, Stream);            \
    } while(0)

# define output_bytes(S, L, Stream)					\
    do {                                                                \
        const unsigned char *__ptr = (const unsigned char*)(void*)(S);  \
        unsigned long __i = 0, __max = (L);                             \
        while(likely(__i < __max)) output_char(__ptr[__i++], Stream);	\
    } while(0)

#else

# ifdef __cplusplus
#  include <iostream>
#  include <iomanip>
#  define output_float(F, Stream, Precision) ((((Stream) == 2)? std::cerr : std::cout) \
					      << std::setprecision(Precision) \
					      << std::scientific \
					      << (double)F << std::flush)
#  define output_char(N, Stream) ((((Stream) == 2)? std::cerr : std::cout) << (char)(N) << std::flush)
#  define output_hex(N, Stream) ((((Stream) == 2)? std::cerr : std::cout) << std::hex << (unsigned long)(N) << std::flush)
#  define output_int(N, Stream) ((((Stream) == 2)? std::cerr : std::cout) << std::dec << (long)(N) << std::flush)
#  define output_uint(N, Stream) ((((Stream) == 2)? std::cerr : std::cout) << std::dec << (unsigned long)(N) << std::flush)
#  define output_string(S, Stream) ((((Stream) == 2)? std::cerr : std::cout) << (const char*)(S) << std::flush)
#  define output_bytes(S, L, Stream) do { \
        (((Stream) == 2)? std::cerr : std::cout).write((const char*)(void*)(S), (L)); \
        (((Stream) == 2)? std::cerr : std::cout) << std::flush; \
    } while(0)
# else
#  include <stdio.h>
#  define output_float(F, Stream, Precision) do { \
        FILE * __s = (Stream) == 2 ? stderr : stdout; \
        fprintf(__s, "%.*le", (Precision), (double)(F)); \
        fflush(__s); \
    } while(0)
#  define output_char(N, Stream) do { \
        FILE * __s = (Stream) == 2 ? stderr : stdout; \
        fprintf(__s, "%c", (char)(N)); \
        fflush(__s); \
    } while(0)
#  define output_hex(N, Stream) do { \
        FILE * __s = (Stream) == 2 ? stderr : stdout; \
        fprintf(__s, "%lx", (unsigned long)(N)); \
        fflush(__s); \
    } while(0)
#  define output_int(N, Stream) do { \
        FILE * __s = (Stream) == 2 ? stderr : stdout; \
        fprintf(__s, "%ld", (long)(N)); \
        fflush(__s); \
    } while(0)
#  define output_uint(N, Stream) do { \
        FILE * __s = (Stream) == 2 ? stderr : stdout; \
        fprintf(__s, "%lu", (unsigned long)(N)); \
        fflush(__s); \
    } while(0)
#  define output_string(S, Stream) do { \
        FILE * __s = (Stream) == 2 ? stderr : stdout; \
        fprintf(__s, "%s", (const char*)(S)); \
        fflush(__s); \
    } while(0)
#  define output_bytes(S, L, Stream) do { \
        FILE * __s = (Stream) == 2 ? stderr : stdout; \
        fwrite((const char*)(S), (L), 1, __s); \
        fflush(__s); \
    } while(0)
# endif

#endif

#endif // ! SLC_SVP_TESTOUTPUT_H
