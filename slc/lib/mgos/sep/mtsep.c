//
// mtsep.c: this file is part of the SL toolchain.
//
// Copyright (C) 2009,2010,2011 Universiteit van Amsterdam.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <svp/sep.h>
#include <svp/compiler.h>
#include <svp/testoutput.h>
#include <svp/delegate.h>

#include "mgsim/mtconf.h"

// log2 of the maximum number of cores that are supported
#define L2_MAX_NCORES 10
#define MAX_NCORES (1 << L2_MAX_NCORES)

struct range_t {
    // Size of the range, if free. 0 if used.
    unsigned int size;
    
    // Links for doubly-linked lists
    struct range_t* next;
    struct range_t* prev;
};

struct sep_data_t {
    // Public SEP info
    struct SEP sep_info;
    
    // Place where the SEP runs
    sl_place_t sep_place;
    
    // Number of bits for an core address. To construct Place IDs.
    unsigned int pid_bits;
    
    // Total number of cores on the grid.
    size_t num_cores;
    
    // Storage for all possible ranges (worst case: 1 core per range)
    struct range_t ranges[MAX_NCORES];
    
    // Doubly linked list of free range per (power of two) size
    struct range_t* free_by_size[L2_MAX_NCORES + 1];
};

static
size_t fast_log2(size_t x) {
    return sizeof(size_t)*CHAR_BIT - __builtin_clzl(x) - 1;
}

static
bool is_pow2(size_t x) {
    return (x & (x - 1)) == 0;
}

#define MAKE_PLACE_ID(sep, id, size, caps) \
    ((size) | ((id) << 1) | ((caps) << ((sep)->pid_bits + 1)))

// Allocate a range of specified (power of two) size from the free ranges, if possible.
static
struct range_t* alloc_range(struct sep_data_t* sep,
                            size_t l2_size)
{
    // find a range of desired size, or larger
    size_t i;
    for (i = l2_size; i <= L2_MAX_NCORES; ++i) {
        struct range_t* r = sep->free_by_size[i];
        if (r) {
            // found a suitable range, mark as used
            r->size = 0;
            
            // remove the range from its index
            sep->free_by_size[i] = r->next;
            
            // split the range until we get the desired size
            while (i > l2_size) {
                --i;
                    
                // add the second half to the free list for its size.
                // also mark the size of the free range so we can merge during free.
                size_t size = 1 << i;
                struct range_t* r2 = r + size;
                    
                r2->size  = size;
                r2->prev  = 0;
                r2->next  = sep->free_by_size[i];
                sep->free_by_size[i] = r2;
            }
            
            // return the core address
            return r;
        }
    }
    return 0;
}

static sl_place_t do_sep_alloc(struct sep_data_t* sep, size_t ncores, unsigned long policy)   
{
    /*
     Note about allocation policies:
     
     SAL_MIN is actually SAL_EXACT with the number of cores rounded up to a
     power of two. When we find a free range that's larger than than desired,
     the architecture allows up to keep splitting that range into two until
     we get the a range of the desired size, thus SAL_MIN and SAL_EXACT share
     the same allocation process.
     
     SAL_DONTCARE right now is a naive implementation that returns just one
     core, since we can always keep splitting available ranges down to one.
     In the future, this might be a smarter strategy that takes topology or
     load into account. It's therefore the same as SAL_EXACT with 1 core.
     
     SAL_MAX tries to allocate its limit and if that fails, tries to allocate
     a smaller range, all the way down to one core.
     */
    struct range_t* r = 0;
    
    switch(policy) {
    case SAL_DONTCARE:
        ncores = 1;
        // fall-through to SAL_MIN
        
    case SAL_MIN:
        // hardware only supports powers of two, so round request up to
        // a power of two
        ncores = (ncores != 0) ? 1U << fast_log2(2 * ncores - 1) : 1;
        // fall-through to SAL_EXACT
        
    case SAL_EXACT:
        // hardware only supports powers of two, so if there's a request
        // for something else, tough luck.
        if (likely(ncores != 0 && is_pow2(ncores))) {
            r = alloc_range(sep, fast_log2(ncores));
        }
        break;

    case SAL_MAX: {
        // hardware only supports powers of two, so round request down to
        // a power of two
        if (likely(ncores != 0)) {
            size_t i = fast_log2(ncores);
            do {
                r = alloc_range(sep, i);
            } while (!r && i--);
            ncores = (1 << i);
        }
        break;
    }
    }

    return r ? MAKE_PLACE_ID(sep, r - sep->ranges, ncores, 1) : 0;
}

// Performs place allocation
sl_def(sep_alloc, void,
    sl_glparm(struct sep_data_t*, sep),
    sl_glparm(size_t, ncores),
    sl_glparm(unsigned long, policy),
    sl_shparm(sl_place_t, result) )
{
    sl_place_t p = do_sep_alloc(sl_getp(sep), sl_getp(ncores), sl_getp(policy));
    sl_setp(result, p);
}
sl_enddef

// Frees a (possibly shared) range
static
void free_range(struct sep_data_t* sep,
                struct range_t* r,
                unsigned int l2_size)
{
    size_t size = 1 << l2_size;

    // keep checking if we can merge with an adjacent range.
    for (; l2_size < L2_MAX_NCORES; ++l2_size, size *= 2)
    {
        // if the range is located on an odd multiple of its size,
        // we merge with left, otherwise, with right.
        bool merge_left = (((r - &sep->ranges[0]) >> l2_size) & 1) != 0;
        struct range_t* r2 = merge_left ? (r - size) : (r + size);
        if (r2->size != size)
            // can't merge
            break;
            
        // remove merged range from its free index
        if (r2->prev)
            r2->prev->next = r2->next;
        else
            sep->free_by_size[l2_size] = r2->next;
        
        if (r2->next)
            r2->next->prev = r2->prev;

        if (merge_left)
            r = r2;
    }

    // add the final free range to its appropriate index.
    // also mark the size of the free range so we can merge during free.
    r->size  = size;
    r->prev  = 0;
    r->next  = sep->free_by_size[l2_size];
    sep->free_by_size[l2_size] = r;
}

sl_def(sep_free, void,
    sl_glparm(struct sep_data_t*, sep),
    sl_glparm(sl_place_t, p) )
{
    // extract the address/size field from the place's PID
    struct sep_data_t* sep = sl_getp(sep);
    sl_place_t p = sl_getp(p);
    
    sl_place_t pid = p & ((1 << (sep->pid_bits + 1)) - 1);
    unsigned int l2_size = __builtin_ctz(pid);
    size_t index = (pid - (1 << l2_size)) / 2;
    
    struct range_t* r = &sep->ranges[index];
    if (r->size != 0)
        // Place is unused, do nothing.
        return;

    free_range(sep, r, l2_size);
}
sl_enddef

static
int root_sep_ctl(struct SEP* sep, unsigned long request, void *a, void *b)
{
    struct sep_data_t* sd = (struct sep_data_t*)(void*)sep;

    switch (request & 0xffff)
    {
    case SEP_ALLOC:
    {
        size_t ncores = (size_t)(long)a;
        size_t policy = request & SAL_EXACT;
    
        sl_create(,sd->sep_place,,,,,sl__exclusive, sep_alloc,
            sl_glarg(struct sep_data_t*,,sd),
            sl_glarg(size_t,,ncores),
            sl_glarg(unsigned long,,policy),
            sl_sharg(sl_place_t, result, 0));
        sl_sync();
        
        sl_place_t p = sl_geta(result);
        if (p) {
            *(sl_place_t*)b = p;
            return 0;
        }
        break;
    }
    
    case SEP_FREE:
    {
        sl_place_t p = *(sl_place_t*)a;
        
        sl_create(,sd->sep_place,,,,,sl__exclusive, sep_free,
            sl_glarg(struct sep_data_t*,,sd),
            sl_glarg(sl_place_t,,p));
        sl_sync();        
        return 0;
    }
    
    case SEP_QUERY:
    {
        sl_place_t p = *(sl_place_t*)a;
        union placeinfo *ret = (union placeinfo*)b;
        
        if (p == 0)
            // Invalid place ID
            break;

        // extract the address/size field from the place's PID
        sl_place_t pid = p & ((1 << (sd->pid_bits + 1)) - 1);
        size_t size = pid & -pid;
        size_t index = (pid & ~size) / 2;
        
        if (size >= sd->num_cores || index > sd->num_cores - size)
            // Place lies outside grid
            break;
        
        if (size == 1)
        {
            // One core-place: atomic, programmable
            ret->flags = 2|4|1;
#if defined(__slc_os_sim__)
            ret->a.p.family_capacity = mgconf_ftes_per_core;
            ret->a.p.thread_capacity = mgconf_ttes_per_core;
            ret->a.p.rate            = mgconf_core_freq;
#else
            // FIXME: this information should be retrieved
            // from the substrate execution environment.
            ret->a.p.family_capacity = 0;
            ret->a.p.thread_capacity = 0; 
            ret->a.p.rate            = 0;
#endif
        }
        else
        {
            // Multi-core place: compound, homogenous
            ret->flags = 4|1;
            ret->c.arity = size;
        }
        return 0;
    }
    
    default:
        // Unimplemented
        break;
    }
    return -1;
}

static struct sep_data_t root_sep_data = { 
    { &root_sep_ctl },
};

struct SEP *root_sep = &root_sep_data.sep_info;

extern int verbose_boot;

extern sl_place_t __main_place_id;

void sep_dump_info(struct SEP* p)
{
    struct sep_data_t *s = (struct sep_data_t*)(void*)p;
    size_t i;

    printf("handling %zu cores, pid_bits = %u\n", s->num_cores, s->pid_bits);
    for (i = 0; i < L2_MAX_NCORES + 1; ++i)
    {
        struct range_t* r;
        printf("free list %zu ", i);
        for (r = s->free_by_size[i]; r; r = r->next)
            printf(" -> (sz:%u,off:%u)",
                   r->size,
                   (unsigned)(r - &(s->ranges[0])));
        putchar('\n');
    }
}

void sys_sep_init(void)
{
    sl_place_t p = get_current_place();
    size_t ncores = p & -p;
    unsigned int i, size;

    if (verbose_boot) {
        output_string("* SEP init: setting up for ", 2);
        output_uint(ncores, 2);
        output_string(" cores...  ", 2);
    }
    
    // can't use assert() before places have been defined
    if (unlikely(ncores > MAX_NCORES)) {
        output_string("SEP init fail: ncores > MAX_NCORES\n", 2);
        abort();
    }

    // right now we only support a homogenous grid with some I/O cores, and don't use the topology
    
    // Get the number of bits needed for an address: ceil(log2(gc->ncores))
    root_sep_data.num_cores = ncores;
    root_sep_data.pid_bits = fast_log2(ncores * 2 - 1);

    // add the available cores to the free pool
    for (i = 0, size = ncores; size != 0; )
    {
        unsigned int l2_size = fast_log2(ncores);
        free_range(&root_sep_data, &root_sep_data.ranges[0], l2_size);
        i    += (1 << l2_size);
        size -= (1 << l2_size);
    }
        
    // Grab a core for the single shared place for the root SEP.
    // Must be core 0.
    p = do_sep_alloc(&root_sep_data, 1, SAL_EXACT);
    if (unlikely(!p)) {
        output_string("SEP init fail: unable to reserve core for root SEP\n", 2);
        abort();
    }
    root_sep_data.sep_place = p;
    
    // share the first place with the root SEP
    __main_place_id = p;

    if (verbose_boot) {
        output_string("done.", 2);
        output_ts(2);
        output_char('\n', 2);
    }
}
