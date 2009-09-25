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

#define MAX_NCORES 1024

struct listnode {
  struct placeinfo pi;
  struct listnode *next;
  int allocated;
};

struct listnode  allplaces[MAX_NCORES] = { { 0 }, 0, 0, 0} ;
struct listnode* pools[MAX_NCORES] = { 0 };

struct placeinfo * sep_place = 0;

// initialization has been completed?
static int init_done = 0;


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
	struct listnode* n = &allplaces[current_ring_id];
	size_t ncores = n->pi.ncores;
	n->next = pools[ncores];
	pools[ncores] = n;
      }

      /* restart a new ring */
      current_ring_id = pc->core_info[i][1];

      /* start configuring the place */
      allplaces[current_ring_id].pi.pid =
	(pc->core_info[i][0] << 3) /* core id */
	| 4 /* delegate, non-exclusive */;
      allplaces[current_ring_id].pi.ncores = 1;
      allplaces[current_ring_id].pi.shared = 0;

      if (!sep_place) {
	allplaces[current_ring_id].pi.shared = 0;
	allplaces[current_ring_id].allocated = 1;
	sep_place = &allplaces[current_ring_id].pi;
      }
    }

    else {
      /* already started the ring, increase the number of cores */
      allplaces[current_ring_id].pi.ncores += 1;
    }
  }
  /* add the last item to its pool */
  struct listnode* n = &allplaces[current_ring_id];
  size_t ncores = n->pi.ncores;
  n->next = pools[ncores];
  pools[ncores] = n;

  init_done = 1;
}

sl_def(sep_alloc,
       void,
       sl_glparm(enum sep_alloc_policy, policy),
       sl_glparm(int, policy_arg),
       sl_shparm(struct placeinfo*, result))
{
  int canshare = !!(sl_getp(policy) & SAL_CANSHARE);
  int i;
  struct listnode *p = 0;

  switch(sl_getp(policy) & 0x3) {
  case SAL_DONTCARE:
    for (i = 0; i < MAX_NCORES; ++i)
      for (p = pools[i]; p; p = p->next)
	if (!p->allocated || (canshare && p->pi.shared)) goto done;

    break;
  case SAL_MIN:
    if (sl_getp(policy_arg) < 0 || sl_getp(policy_arg) > MAX_NCORES) break;
    for (i = sl_getp(policy_arg); i < MAX_NCORES; ++i)
      for (p = pools[i]; p; p = p->next)
	if (!p->allocated || (canshare && p->pi.shared)) goto done;

    break;
  case SAL_MAX:
    if (sl_getp(policy_arg) < 0 || sl_getp(policy_arg) > MAX_NCORES) break;
    for (i = sl_getp(policy_arg); i > 0; --i)
      for (p = pools[i]; p; p = p->next)
	if (!p->allocated || (canshare && p->pi.shared)) goto done;

    break;
  case SAL_EXACT:
    if (sl_getp(policy_arg) < 0 || sl_getp(policy_arg) > MAX_NCORES) break;
    for (p = pools[sl_getp(policy_arg)]; p; p = p->next)
      if (!p->allocated || (canshare && p->pi.shared)) goto done;

    break;
  }
 done:
  if (p) {
    p->allocated = 1;
    p->pi.shared = canshare;
  }
  sl_setp(result, &p->pi);
}
sl_enddef

sl_def(sep_free, void, sl_glparm(struct placeinfo*, p));
{
  struct listnode* n = (struct listnode*)(void*)sl_getp(p);
  n->allocated = 0;
}
sl_enddef

sl_def(sep_dump_info, void)
{
  int i;
  puts("PID\t#cores\tallocated\tshared\n");
  for (i = 0; i < MAX_NCORES; ++i)
    if (allplaces[i].pi.pid)
      printf("0x%x\t%u\t%d\t%d\n",
	     allplaces[i].pi.pid,
	     allplaces[i].pi.ncores,
	     allplaces[i].allocated,
	     allplaces[i].pi.shared);
}
sl_enddef

// global variable definition
struct placeinfo* sep_place;
