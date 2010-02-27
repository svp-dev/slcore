//
// heap.c: this file is part of the SL toolchain.
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
#include <cerrno.h>
#include <svp/testoutput.h>
#include "heap.h"

#ifndef PAGESIZE
#define PAGESIZE 1024*1024*1024
#endif

#ifndef HEAP_SIZE
#define HEAP_SIZE 10L*1024*1024*1024
#endif

#ifndef PAGE_BITS
#define PAGE_BITS 30
#endif

char HEAP[HEAP_SIZE] __attribute__ ((aligned(PAGESIZE)));
static char *brk_p = HEAP;

void *brk(const void *p)
{
    unsigned long heap_base = (unsigned long)(void*)&(HEAP[0]);
    unsigned long addr = (unsigned long)p;

    if (addr & ((1ULL << PAGE_BITS) - 1))
        addr = ((addr >> PAGE_BITS) + 1) << PAGE_BITS;

    if (addr < heap_base || addr >= (heap_base + sizeof(HEAP)))
    {
        errno = ENOMEM;
        return (void*)-1;
    }
    return brk_p = (char*)(void*)addr;
}

void* sbrk(int incr)
{
    return brk(brk_p + incr);
}

extern int verbose_boot;

void sys_heap_init(void)
{
    if (!verbose_boot)
        return;
    
    output_string("* heap at 0x", 2);
    output_hex(brk_p, 2);
    output_char(',', 2);
    output_char(' ', 2);
    output_uint(HEAP_SIZE, 2);
    output_string(" bytes.\n", 2);
}
