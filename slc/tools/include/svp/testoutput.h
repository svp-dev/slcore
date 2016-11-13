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

#elif defined(__slc_os_fpga__)

#include <svp/divide.h>

#if !defined(TESTOUTPUT_IMPL)
#define EXTERNTO extern
#else
#define EXTERNTO /*nothing*/
#endif

#define DEBUG_STDOUT_ADDR 0x10
#define DEBUG_STDERR_ADDR 0x20
#define DEBUG_FMT_BYTE 0x00
#define DEBUG_FMT_INT  0x04
#define DEBUG_FMT_UINT 0x08
#define DEBUG_FMT_HEX  0x0c
#define DEBUG_CTL_ADDR  0x00
#define DEBUG_CTL_EXIT  0x0  
#define DEBUG_CTL_ABORT  0x4  
#define DEBUG_CHAN_ADDR(C) (0x10 * (C))

#define DEBUG_OUTPUT(Addr, Value) do {					\
	__asm__ __volatile__("sta %0, [%1] 0x84" : : "r"(Value), "r"(Addr));	\
    } while(0)								\
    
#ifdef DEBUG_COPY_OUTPUT_TO_BUFFER
extern char __dbgbuf[];
#define __DBGBUF_SIZE 1024
extern unsigned long __dbgbuf_p;
#define DEBUG_COPY_TO_BUFFER(Char) do {		\
	unsigned long p = __dbgbuf_p;		\
	__dbgbuf[p++] = Char;			\
	p %= __DBGBUF_SIZE;			\
	__dbgbuf[p] = 0;			\
	__dbgbuf_p = p;				\
    } while(0)    
#else
#define DEBUG_COPY_TO_BUFFER(Char) (void)0
#endif

#define output_char(byte, chan) do {			      \
	DEBUG_OUTPUT(DEBUG_CHAN_ADDR(chan)|DEBUG_FMT_BYTE, byte);	\
	DEBUG_COPY_TO_BUFFER(byte);					\
    } while(0)

#define output_uint(val, chan) do {			     \
	DEBUG_OUTPUT(DEBUG_CHAN_ADDR(chan)|DEBUG_FMT_UINT, val);	\
	DEBUG_COPY_TO_BUFFER('%');					\
    } while(0)

#define output_int(val, chan) do {			    \
	DEBUG_OUTPUT(DEBUG_CHAN_ADDR(chan)|DEBUG_FMT_INT, val); \
	DEBUG_COPY_TO_BUFFER('%');				\
    } while(0)

#define output_hex(val, chan) do {				\
	DEBUG_OUTPUT(DEBUG_CHAN_ADDR(chan)|DEBUG_FMT_HEX, val);	\
	DEBUG_COPY_TO_BUFFER('%');				\
    } while(0)


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
