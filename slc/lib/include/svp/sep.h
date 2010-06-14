//
// sep.h: this file is part of the SL toolchain.
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

#ifndef SLC_SVP_SEP_H
# define SLC_SVP_SEP_H

#include <svp/delegate.h>

#ifdef __mt_freestanding__
#define SVP_HAS_SEP 1

#include <stddef.h>
#include <stdint.h>

// information returned by sep_alloc etc.
struct placeinfo {
    // identifier for the create() statement, waits until place become free
    sl_place_t  pid;
    // identifier for the create() statement, may fail
    sl_place_t  soft_pid;
    // number of cores in the place
    uint16_t ncores;
    // number of family table entries per core
    uint16_t nfamilies_per_core;
    // number of thread table entries per core
    uint16_t nthreads_per_core;
};

// place where all the sep_alloc calls should
// be delegated to.
extern sl_place_t sep_place;

// allocation policies
enum sep_alloc_policy {
  /* flags that select the number of cores */
  SAL_DONTCARE = 0,   // any #cores will do
  SAL_MIN = 1UL << 62,        // at least specified #cores avail
  SAL_MAX = 2UL << 62,        // at most specified #cores avail
  SAL_EXACT = SAL_MIN | SAL_MAX,   // exactly specified #cores
};

struct SEP {
  sl_place_t sep_place;

  // allocation routine
  sl_decl_fptr(sep_alloc,
               void,
               sl_glparm(struct SEP*, sep),
               sl_glparm(unsigned long, policy),
               sl_shparm(struct placeinfo*, result));

  // deallocation routine
  sl_decl_fptr(sep_free, 
               void, 
               sl_glparm(struct SEP*, sep),
               sl_glparm(struct placeinfo*, p));

  // status routine
  sl_decl_fptr(sep_dump_info,
               void,
               sl_glparm(struct SEP*, sep));
};

extern struct SEP* root_sep;

// to be called once (in the init routine)
void sys_sep_init(void* init_parameters);

#else
#define SVP_HAS_SEP 0
#endif

#endif // ! SLC_SVP_SEP_H
