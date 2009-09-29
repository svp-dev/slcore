//
// mtsep.c: this file is part of the SL toolchain.
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

#include <cstdint.h>
#include <svp/sep.h>
#include <svp/assert.h>
#include <svp/iomacros.h>
#include <svp/compiler.h>

#define MAX_NCORES 1024

struct listnode {
  struct placeinfo pi;
  struct listnode *next;
  int allocated;
};

struct sep_data_t {
  struct SEP sep_info;  
  struct listnode  allplaces[MAX_NCORES];
  struct listnode* pools[MAX_NCORES];
};


sl_def(sep_alloc,
       void,
       sl_glparm(struct SEP*, sep),
       sl_glparm(enum sep_alloc_policy, policy),
       sl_shparm(struct placeinfo*, result))
{
  struct sep_data_t* sd = (struct sep_data_t*)(void*)sl_getp(sep);
  int i;
  struct listnode *p = 0;
  unsigned long policy = sl_getp(policy) & SAL_EXACT;
  unsigned long policy_arg = sl_getp(policy) & 0x3ff;
  if (policy == SAL_DONTCARE) {
    for (i = 0; i < MAX_NCORES; ++i)
      for (p = sd->pools[i]; p; p = p->next)
	if (!p->allocated) goto done;
  } else {
    if (likely(policy_arg <= MAX_NCORES)) {
      if (policy == SAL_MIN) {
	for (i = policy_arg; i < MAX_NCORES; ++i)
	  for (p = sd->pools[i]; p; p = p->next)
	    if (!p->allocated) goto done;
      } else if (policy == SAL_MAX) {
	for (i = policy_arg; i > 0; --i)
	  for (p = sd->pools[i]; p; p = p->next)
	    if (!p->allocated) goto done;
      } else if (policy == SAL_EXACT) {
	for (p = sd->pools[policy_arg]; p; p = p->next)
	  if (!p->allocated) goto done;
      }
    }
  }

 done:
  if (p) 
    p->allocated = 1;
  sl_setp(result, &p->pi);
}
sl_enddef

sl_def(sep_free, void, 
       sl_glparm(struct SEP*, sep),
       sl_glparm(struct placeinfo*, p));
{
  struct listnode* n = (struct listnode*)(void*)sl_getp(p);
  n->allocated = 0;
}
sl_enddef

sl_def(sep_dump_info, void,
       sl_glparm(struct SEP*, sep))
{
  int i;
  struct sep_data_t* sd = (struct sep_data_t*)(void*)sl_getp(sep);
  puts("PID\t#cores\tallocated\tshared\n");
  for (i = 0; i < MAX_NCORES; ++i)
    if (sd->allplaces[i].pi.pid)
      printf("0x%x\t%u\t%d\n",
	     sd->allplaces[i].pi.pid,
	     sd->allplaces[i].pi.ncores,
	     sd->allplaces[i].allocated);
}
sl_enddef

static struct sep_data_t root_sep_data = { 
  { 0, 
    &sep_alloc,
    &sep_free,
    &sep_dump_info, },
  { 0 },
  { 0 }
};

struct SEP *root_sep = &root_sep_data.sep_info;

void sep_init(void* init_parameters)
{
  struct placeconf {
    uint32_t   ncores;
    uint16_t core_info[][2];
  };
  struct placeconf * restrict pc = (struct placeconf*) init_parameters;

  svp_assert(pc->ncores <= MAX_NCORES);

  size_t i;
  int current_ring_id = -1;

  for (i = 0; i < pc->ncores; ++i) {
    if (pc->core_info[i][1] != current_ring_id) {
      /* starting a new ring */

      /* first check if we were looking at a ring before */
      if (current_ring_id > 0) {
	/* yes, add it to its pool */
	struct listnode* n = &root_sep_data.allplaces[current_ring_id];
	size_t ncores = n->pi.ncores;
	n->next = root_sep_data.pools[ncores];
	root_sep_data.pools[ncores] = n;
      }

      /* restart a new ring */
      current_ring_id = pc->core_info[i][1];

      /* start configuring the place */
      root_sep_data.allplaces[current_ring_id].pi.pid =
	(pc->core_info[i][0] << 3) /* core id */
	| 4 /* delegate, non-exclusive */;
      root_sep_data.allplaces[current_ring_id].pi.ncores = 1;

      if (!root_sep_data.sep_info.sep_place) {
	root_sep_data.allplaces[current_ring_id].allocated = 1;
	root_sep_data.sep_info.sep_place = root_sep_data.allplaces[current_ring_id].pi.pid;
      }
    }

    else {
      /* already started the ring, increase the number of cores */
      root_sep_data.allplaces[current_ring_id].pi.ncores += 1;
    }
  }
  /* add the last item to its pool */
  struct listnode* n = &root_sep_data.allplaces[current_ring_id];
  size_t ncores = n->pi.ncores;
  n->next = root_sep_data.pools[ncores];
  root_sep_data.pools[ncores] = n;

}
