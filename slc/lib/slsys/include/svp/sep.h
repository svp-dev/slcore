//
// sep.h: this file is part of the SL toolchain.
//
// Copyright (C) 2009,2010,2011 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#ifndef SLC_SVP_SEP_H
# define SLC_SVP_SEP_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <svp/delegate.h>

#define SVP_HAS_SEP 1

struct SEP {
    // SEP interface: returns -1 on error
    int   (*ctl)(struct SEP* self, unsigned long request, void* a, void *b);
};

extern struct SEP* root_sep;

// Place allocation and deallocation
//
#define SEP_ALLOC     1  /* a: policy arg, b: sl_place_t* */
#define SEP_FREE      2  /* a: sl_place_t* */
#define SEP_QUERY     3  /* a: sl_place_t*, b: union placeinfo* */ 

// Allocation policy: ORed with SEP_ALLOC
#define SAL_DONTCARE  0
#define SAL_NCORES    0x00010000
#define SAL_MAX       (1UL<<30|SAL_NCORES) 
#define SAL_MIN       (2UL<<30|SAL_NCORES)
#define SAL_EXACT     (SAL_MAX|SAL_MIN)

union placeinfo {
    unsigned long      flags;

    struct {
        unsigned long  flags;
        size_t         arity;
    }                  c;

    struct {
        unsigned long  flags;
        struct {
            size_t     family_capacity;
            size_t     thread_capacity;
            size_t     rate;
        }              p;
    }                  a;
};

#define SP_IS_HARDWARE(PI)         ((PI)->flags & 0x1)

//      For hardware places:
#define   SP_IS_ATOMIC(PI)           ((PI)->flags & 0x2)
#define   SP_IS_COMPOUND(PI)         (!SP_IS_ATOMIC(PI))

//        For atomic places:
#define     SP_IS_PROGRAMMABLE(PI)     ((PI)->flags & 0x4)

//        For compound places:
#define     SP_IS_HOMOGENEOUS(PI)      ((PI)->flags & 0x4)


// Helper macros:
#define sep_alloc(SEP, PlacePtr, Policy, Arg) \
    (SEP)->ctl((SEP), SEP_ALLOC|(Policy), (void*)(long)(Arg), PlacePtr)
#define sep_free(SEP, PlacePtr) \
    (SEP)->ctl((SEP), SEP_FREE, PlacePtr, 0)
#define sep_query(SEP, PlacePtr, Arg) \
    (SEP)->ctl((SEP), SEP_QUERY, PlacePtr, (void*)(Arg))


#endif // ! SLC_SVP_SEP_H
