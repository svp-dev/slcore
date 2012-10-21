//
// heap.c: this file is part of the SL toolchain.
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
#include <errno.h>
#include <assert.h>
#include <svp/testoutput.h>

#include "heap.h"
#include "mgsim/mtconf.h"

#if defined(__mtsparc__)||defined(__mips__)

#ifndef PAGESIZE
#define PAGESIZE 4*1024
#endif

#ifndef HEAP_SIZE
#define HEAP_SIZE 40*1024*1024
#endif

#ifndef PAGE_BITS
#define PAGE_BITS 12
#endif

#else

#ifndef PAGESIZE
#define PAGESIZE 32*1024
#endif

#ifndef HEAP_SIZE
#define HEAP_SIZE 10L*1024*1024*1024
#endif

#ifndef PAGE_BITS
#define PAGE_BITS 15
#endif
#endif

#if defined(__slc_os_sim__)

/* pre-allocate 1 byte; the map/unmap logic will use the next page in the
   address space as a base. */
static char __heap[1] __attribute__ ((aligned(PAGESIZE), section("MGOS_HEAPBASE")));
static char *__heapbase = __heap + PAGESIZE;
static char *brk_p = __heap + PAGESIZE;

#else

/* no MGSim MMU; pre-allocate statically. */
static char __heap[HEAP_SIZE] __attribute__ ((aligned(PAGESIZE)));
static char *__heapbase = __heap;
static char *brk_p = __heap;

#endif


void *brk(const void *p)
{
    unsigned long heap_base = (unsigned long)(void*)__heapbase;
    unsigned long addr = (unsigned long)p;

    if (addr & ((1ULL << PAGE_BITS) - 1))
        addr = ((addr >> PAGE_BITS) + 1) << PAGE_BITS;

    if (addr < heap_base || addr >= (heap_base + HEAP_SIZE))
    {
        errno = ENOMEM;
        return (void*)-1;
    }


#if defined(__slc_os_sim__)
    // Unmap from MMU if reducing, map back if increasing.
    assert(1UL << PAGE_BITS == PAGESIZE);
    assert(PAGE_BITS >= 12 && (PAGE_BITS - 12) < 8);

    unsigned long cur_addr = (unsigned long)(void*)brk_p;
    while (cur_addr > addr)
    {
        cur_addr -= PAGESIZE;
        mgsim_control(cur_addr, MGSCTL_TYPE_MEM, MGSCTL_MEM_UNMAP, PAGE_BITS-12);
    }
    while (addr > cur_addr)
    {
        mgsim_control(cur_addr, MGSCTL_TYPE_MEM, MGSCTL_MEM_MAP, PAGE_BITS-12);
        cur_addr += PAGESIZE;
    }
#endif

    return brk_p = (char*)(void*)addr;
}

void* sbrk(int incr)
{
    char *prev = brk_p;
    char *inv = (void*)-1;

    char *ret;
    if (incr == 0)
        ret = brk_p;
    else
        ret = (brk(brk_p + incr) == inv) ? inv : prev;

    return ret;
}

void sys_heap_init(void)
{
    if (!verbose_boot)
        return;
    
    output_string("* heap at 0x", 2);
    output_hex((unsigned long)__heapbase, 2);
    output_string(", max ", 2);
    output_uint(HEAP_SIZE, 2);
    output_string(" bytes.", 2);
    output_ts(2);
    output_char('\n', 2);
}
