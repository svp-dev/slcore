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

# define output_string(S, Stream)                                       \
    do {								\
        const unsigned char *__ptr = (const unsigned char*)(const void*)(S);  \
        while(likely(*__ptr)) output_char(*__ptr++, Stream);            \
    } while(0)

# define output_bytes(S, L, Stream)					\
    do {                                                                \
        const unsigned char *__ptr = (const unsigned char*)(const void*)(S);  \
        unsigned long __i = 0, __max = (unsigned long)(L);              \
        while(likely(__i < __max)) output_char(__ptr[__i++], Stream);	\
    } while(0)

#ifdef __slc_os_sim__

#ifndef __MGSIM_DEBUG_STDOUT
#define __MGSIM_DEBUG_STDOUT 0x200UL
#endif
#ifndef __MGSIM_DEBUG_STDERR
#define __MGSIM_DEBUG_STDERR 0x230UL
#endif

#define output_float(F, Stream, Precision)                              \
    do  {                                                               \
        volatile long *base = (long*)(void*)(((Stream) == 1) ? __MGSIM_DEBUG_STDOUT : __MGSIM_DEBUG_STDERR); \
        *(base + 4) = (long)(Precision);                                \
        __asm__ __volatile__("" : : : "memory");                        \
        *(double*)(void*)(base + 5) = (double)(F);                      \
        __asm__ __volatile__("" : : : "memory");                        \
    } while(0)

#define output_uint(N, Stream)                                          \
    do  {                                                               \
        volatile unsigned long *base = (unsigned long*)(void*)(((Stream) == 1) ? __MGSIM_DEBUG_STDOUT : __MGSIM_DEBUG_STDERR); \
        *(base + 1) = (unsigned long)(N);                               \
        __asm__ __volatile__("" : : : "memory");                        \
    } while(0)

#define output_int(N, Stream)                                           \
    do  {                                                               \
        volatile long *base = (long*)(void*)(((Stream) == 1) ? __MGSIM_DEBUG_STDOUT : __MGSIM_DEBUG_STDERR); \
        *(base + 2) = (long)(N);                                        \
        __asm__ __volatile__("" : : : "memory");                        \
    } while(0)

#define output_hex(N, Stream)                                           \
    do  {                                                               \
        volatile unsigned long *base = (unsigned long*)(void*)(((Stream) == 1) ? __MGSIM_DEBUG_STDOUT : __MGSIM_DEBUG_STDERR); \
        *(base + 3) = (unsigned long)(N);                               \
        __asm__ __volatile__("" : : : "memory");                        \
    } while(0)

#define output_char(N, Stream)                                          \
    do  {                                                               \
        volatile unsigned long *base = (unsigned long*)(void*)(((Stream) == 1) ? __MGSIM_DEBUG_STDOUT : __MGSIM_DEBUG_STDERR); \
        *base = (unsigned long)(N);                                     \
        __asm__ __volatile__("" : : : "memory");                        \
    } while(0)

#elif defined(__slc_arch_leon2mt__) && defined(__slc_os_fpga__)

extern const char __dbgfmt_digits[];
extern char __dbgout_bytes;
extern char __dbgerr_bytes;
extern char __dbgbuf[];
extern volatile int __dbg_exit_status;
#define __DBGBUF_SIZE 1024
extern unsigned long __dbgbuf_p;

#include <svp/divide.h>

#if !defined(TESTOUTPUT_IMPL)
#define EXTERNTO extern
#else
#define EXTERNTO /*nothing*/
#endif

__attribute__((gnu_inline,optimize(3))) EXTERNTO inline
void output_char(int byte, int chan) {
    volatile char * __restrict__ c = (chan == 1 ? &__dbgout_bytes : &__dbgerr_bytes);
    *c = byte;
    unsigned long p = __dbgbuf_p;
    __dbgbuf[p++] = byte;
    p %= __DBGBUF_SIZE;
    __dbgbuf[p] = 0;
    __dbgbuf_p = p;
}

__attribute__((gnu_inline,optimize(3))) EXTERNTO inline
void output_fmt_uint(uint32_t x, int chan, const uint32_t base)
{
    if (x < base) output_char(__dbgfmt_digits[x], chan);
    else {
	uint32_t root = 1;
	while (__divu_uint32_t(x, root) >= base)
	    root *= base;
	while (root) {
	    uint32_t rs, rm;
	    __divmodu_uint32_t(x, root, &rs, &rm);
	    x = rm;
	    root = __divu_uint32_t(root, base);
	    output_char(__dbgfmt_digits[rs], chan);	
	}
    }
}

__attribute__((gnu_inline,optimize(3))) EXTERNTO inline
void output_fmt_int(int32_t x, int chan, const int32_t base) {
    if (!x) output_char('0', chan);
    else {
	int32_t root;
	if (x < 0) {
	    root = -1;
	    output_char('-', chan);
	} else root = 1;
	while (__divs_int32_t(x,root) >= base)
	    root *= base;
	while (root) {
	    int32_t rs, rm;
	    __divmods_int32_t(x, root, &rs, &rm);
	    x = rm;
	    root = __divs_int32_t(root, base);
	    output_char(__dbgfmt_digits[rs], chan);	
	}
    }
}

#define output_uint(N, Stream) output_fmt_uint((unsigned long)(N), Stream, 10)
#define output_hex(N, Stream) output_fmt_uint((unsigned long)(N), Stream, 16)
#define output_int(N, Stream) output_fmt_int((long)(N), Stream, 10)

#else
#error Unsupported arch/os back-end

#endif

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
