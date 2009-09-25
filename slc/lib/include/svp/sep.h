//
// sep.h: this file is part of the SL toolchain.
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

#ifndef SLC_SVP_SEP_H
# define SLC_SVP_SEP_H

#include <cstddef.h>

// information returned by sep_alloc etc.
struct placeinfo {
  // identifier for the create() statement
  sl_place_t  pid;
  // number of cores in the place
  size_t ncores;
  // set if the place is shared by multiple requesters
  int shared;
};

// place where all the sep_alloc calls should
// be delegated to.
extern struct placeinfo * sep_place;

// allocation policies
enum sep_alloc_policy {
  /* flags that select the number of cores */
  SAL_DONTCARE = 0,   // any #cores will do
  SAL_MIN = 1,        // at least specified #cores avail
  SAL_MAX = 2,        // at most specified #cores avail
  SAL_EXACT = SAL_MIN | SAL_MAX,   // exactly specified #cores

  /* other policy flags */
  SAL_CANSHARE = 4
};

// allocation routine
sl_decl(sep_alloc,
	void,
	sl_glparm(enum sep_alloc_policy, policy),
	sl_glparm(int, policy_arg),
	sl_shparm(struct placeinfo*, result));

// deallocation routine
sl_decl(sep_free, void, sl_glparm(struct placeinfo*, p));

// status routine
sl_decl(sep_dump_info, void);

// to be called once (in the init routine)
void sep_init(void* init_parameters);


#endif // ! SLC_SVP_SEP_H
