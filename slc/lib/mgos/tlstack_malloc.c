//
// tlstack_malloc.c: this file is part of the SL toolchain.
//
// Copyright (C) 2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//
#include <stddef.h>
#include <stdint.h>
#include <svp/compiler.h>
#include <svp/testoutput.h>
#include <stdio.h>

/*
 * tls_malloc(): stack-like allocation using thread-local storage.
 * 
 * This is an experimental allocation strategy designed by Carl
 * Joslin & Stephan Herhut @ University of Hertfordshire.
 *
 * It serves allocation requests by stacking the requested chunks
 * in the bottom of the stack area of the TLS. All the memory
 * allocated this way is freed at once when the thread terminates.
 */

void tls_malloc_init(void)
{ /* empty */ }

void tls_malloc_cleanup(void)
{ /* empty */ }

#define ALIGN 16

void* tls_malloc(size_t sz)
{
#ifdef DEBUG_MGSIM
    output_uint(sz, 0);
#endif
    uintptr_t tls_top, tls_bottom;
#if defined(__mips__)
    asm("mfc2 %0, $5" : "=r"(tls_top));
    asm("mfc2 %0, $4" : "=r"(tls_bottom));
#else
    __asm__("ldfp %0" : "=r"(tls_top));
    __asm__("ldbp %0" : "=r"(tls_bottom));
#endif

    // The "stack" area of TLS starts from the middle. Everything
    // below is "thread heap" and is not automatically deallocated
    // when the thread terminates.

    // the computation of stack_base should deal with
    // 1) the highest bit is set (overflow occurs)
    // 2) for the last thread on the last core, tls_top = 0
    uintptr_t stack_base = tls_bottom + (tls_top-tls_bottom)/2;

    char **stack_heap_ptr = (char**)stack_base;

    char *lptr = *stack_heap_ptr;
    if (unlikely(lptr == 0))
        /* not yet been there, need to init the top pointer */
        lptr = (char*)(stack_base + ALIGN);

    /* ensure that size is a multiple of ALIGN */
    if (likely(0 != (sz & (ALIGN-1))))
        sz += ALIGN;
    sz &= ~(ALIGN-1);

    /* allocate */
    *stack_heap_ptr = lptr + sz;
#ifdef DEBUG_MGSIM
    output_hex(lptr, 0);
#endif
    return (void*)lptr;
}
