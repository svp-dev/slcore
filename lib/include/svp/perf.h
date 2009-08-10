//
// perf.slh: this file is part of the slc project.
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
#ifndef __SVP_PERF_H__
# define __SVP_PERF_H__

#include <cstdint.h>

#if defined (__i386__)

static __inline__ int64_t get_cycles(void) __attribute__((always_inline)) 
{
  int32_t __res__[2];
#if defined (_LP64)                      // 64 bit mode
  __asm__ __volatile__  (               // serialize (save rbx)
      "xorl %%eax,%%eax \n push %%rbx \n cpuid \n"
      ::: "%rax", "%rcx", "%rdx");
  __asm__ __volatile__  (               // read TSC, store edx:eax in res
      "rdtsc\n"
      : "=a" (__res__[0]), "=d" (__res__[1]) );
  __asm__ __volatile__  (               // serialize again
      "xorl %%eax,%%eax \n cpuid \n pop %%rbx \n"
      ::: "%rax", "%rcx", "%rdx");
#else                                    // 32 bit mode
  __asm__ __volatile__  (               // serialize (save ebx)
      "xorl %%eax,%%eax \n pushl %%ebx \n cpuid \n"
      ::: "%eax", "%ecx", "%edx");
  __asm__ __volatile__  (               // read TSC, store edx:eax in res
      "rdtsc\n"
      : "=a" (__res__[0]), "=d" (__res__[1]) );
  __asm__ __volatile__  (               // serialize again
      "xorl %%eax,%%eax \n cpuid \n popl %%ebx \n"
      ::: "%eax", "%ecx", "%edx");
#endif
  return *(int64_t*)__res__;
}

#elif defined(__alpha__) || defined(__mtalpha__)

#define get_cycles() ({ \
      int64_t __cycles_c__;					      \
      __asm__ __volatile__ ("rpcc %0" : "=r"(__cycles_c__) : : "memory"); \
      __cycles_c__;							\
    })

#else

# warning Reading the time stamp counter is not defined on your system.
#define get_cycles() ((int64_t)0)

#endif

#endif
