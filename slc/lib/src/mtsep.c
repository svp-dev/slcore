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
#define L2_MAX_NCORES 10

struct listnode {
  struct placeinfo pi;
  struct listnode *next;
  unsigned long l2_ncores;
  int allocated;
};

struct sep_data_t {
  struct SEP sep_info;  
  size_t last_nonempty_pool;
  struct listnode  allplaces[MAX_NCORES];
  struct listnode* pools[L2_MAX_NCORES+1];
  int irregular;
};

extern const struct placeinfo * __main_placeinfo;
extern sl_place_t __main_place_id;

__attribute__((always_inline))
unsigned long fast_log2(unsigned long ncores) {
  /*
  switch(ncores) {
  case 0 ... 1: return 0;
  case 2 ... 3: return 1;
  case 4 ... 7: return 2;
  case 8 ... 15: return 3;
  default:
    return 4;
  }
  */
    
  if (ncores >= 128) return 7;
  if (ncores >= 64) return 6;
  if (ncores >= 32) return 5;
  if (ncores >= 16) return 4;
  if (ncores >= 8) return 3;
  if (ncores >= 4) return 2;
  if (ncores >= 2) return 1;
  return 0; 
  //  register unsigned l = 0;
  //  while (ncores >>= 1) ++l;
  // return l;
}


sl_def(sep_alloc,
       void,
       sl_glparm(struct SEP*, sep),
       sl_glparm(enum sep_alloc_policy, policy),
       sl_shparm(struct placeinfo*, result))
{
  struct sep_data_t* sd = (struct sep_data_t*)(void*)sl_getp(sep);
  int i;
  struct listnode *p = 0;
  struct listnode **prev = 0;
  unsigned long policy = sl_getp(policy) & SAL_EXACT;
  unsigned long policy_arg = sl_getp(policy) & 0x3ff;

  size_t lp = sd->last_nonempty_pool;
  if (policy == SAL_DONTCARE) {
    for (i = 0; i <= lp; ++i) {
      prev = &sd->pools[i];
      p = sd->pools[i]; 
      if (p) goto done_yes;
    }
  } else {
    unsigned long l2_pa = fast_log2(policy_arg);
    int irregular = sd->irregular;

    if (policy == SAL_EXACT) {
      if (unlikely(l2_pa > lp)) goto done_no;
      if (likely(!irregular)) {
	if (unlikely(policy_arg != (1 << l2_pa))) goto done_no;
	prev = &sd->pools[l2_pa];
	p = sd->pools[l2_pa]; 
	if (p) goto done_yes;
      } else {
	for (prev = &sd->pools[l2_pa], p = sd->pools[l2_pa]; p; prev = &p->next, p = p->next)
	  if (p->pi.ncores == policy_arg) goto done_yes;
      }
    } else if (policy == SAL_MIN) {
      if (likely(!irregular)) {
	if (unlikely(policy_arg != (1 << l2_pa))) ++l2_pa;
	for (i = l2_pa; i <= lp; ++i) {
	  prev = &sd->pools[i];
	  p = sd->pools[i]; 
	  if (p) goto done_yes;
	}
      } else {
	for (i = l2_pa; i <= lp; ++i)
	  for (prev = &sd->pools[i], p = sd->pools[i]; p; prev = &p->next, p = p->next)
	    if (p->pi.ncores >= policy_arg) goto done_yes;
      }
    } else if (policy == SAL_MAX) {
      if (unlikely(l2_pa > lp)) goto done_no;
      if (likely(!irregular))
	for (i = l2_pa; i >= 0; --i) {
	  prev = &sd->pools[i];
	  p = sd->pools[i]; 
	  if (p) goto done_yes;
	}
      else 
	for (i = l2_pa; i >= 0; --i)
	  for (prev = &sd->pools[i], p = sd->pools[i]; p; prev = &p->next, p = p->next)
	    if (p->pi.ncores <= policy_arg) goto done_yes;
    }
  }

 done_no:
  sl_setp(result, (void*)0);  
  sl_end_thread;
 done_yes:
  p->allocated = 1;
  *prev = p->next;
  
  sl_setp(result, &p->pi);
}
sl_enddef

sl_def(sep_free, void, 
       sl_glparm(struct SEP*, sep),
       sl_glparm(struct placeinfo*, p));
{
  struct sep_data_t* sd = (struct sep_data_t*)(void*)sl_getp(sep);
  struct listnode* n = (struct listnode*)(void*)sl_getp(p);

  size_t l2_ncores = n->l2_ncores;
  n->next = sd->pools[l2_ncores];
  sd->pools[l2_ncores] = n; 
  n->allocated = 0;
}
sl_enddef

sl_def(sep_dump_info, void,
       sl_glparm(struct SEP*, sep))
{
  int i;
  struct sep_data_t* sd = (struct sep_data_t*)(void*)sl_getp(sep);
  puts("PID\t#cores\tallocated\n");
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
  0,
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
  size_t max_l2 = 0;
  int irregular = 0;

  for (i = 0; i < pc->ncores; ++i) {
    if (pc->core_info[i][1] != current_ring_id) {
      /* starting a new ring */

      /* first check if we were looking at a ring before */
      if (current_ring_id > 0) {
	/* yes, add it to its pool */
	struct listnode* n = &root_sep_data.allplaces[current_ring_id];
	size_t ncores = n->pi.ncores;
	unsigned long l2_nc = fast_log2(ncores);
	n->l2_ncores = l2_nc;
	n->next = root_sep_data.pools[l2_nc];
	root_sep_data.pools[l2_nc] = n;
	if (ncores != (1 << l2_nc)) irregular = 1;
	if (max_l2 < l2_nc) max_l2 = l2_nc;
      }

      /* restart a new ring */
      current_ring_id = pc->core_info[i][1];

      /* start configuring the place */
      root_sep_data.allplaces[current_ring_id].pi.pid =
	(pc->core_info[i][0] << 3) /* core id */
	| 4 /* delegate, non-exclusive */;
      root_sep_data.allplaces[current_ring_id].pi.ncores = 1;

      if (!root_sep_data.sep_info.sep_place) {
	  // first ring
	  // FIXME: if running on first core, pid = 0 so this may be reached multiple times.
          root_sep_data.allplaces[current_ring_id].allocated = 1;
          __main_place_id = root_sep_data.sep_info.sep_place = root_sep_data.allplaces[current_ring_id].pi.pid;
          __main_placeinfo = &root_sep_data.allplaces[current_ring_id].pi;
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
  unsigned long l2_nc = fast_log2(ncores);
  n->l2_ncores = l2_nc;
  n->next = root_sep_data.pools[l2_nc];
  root_sep_data.pools[l2_nc] = n;
  if (ncores != (1 << l2_nc)) irregular = 1;
  if (max_l2 < l2_nc) max_l2 = l2_nc;

  root_sep_data.last_nonempty_pool = max_l2;
  root_sep_data.irregular = irregular;
}
