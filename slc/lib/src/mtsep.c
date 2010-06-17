//
// mtsep.c: this file is part of the SL toolchain.
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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <svp/sep.h>
#include <svp/compiler.h>
#include <svp/testoutput.h>
#include "mtconf.h"

#define MAX_NCORES 1024
#define L2_MAX_NCORES 10

struct listnode {
    struct placeinfo pi;
    struct listnode *next;
    size_t l2_ncores;
    size_t sharecount;
    sl_place_t pid;
    sl_place_t soft_pid;
};

struct sep_data_t {
    struct SEP sep_info;  
    size_t last_nonempty_pool;
    struct listnode  allplaces[MAX_NCORES];
    struct listnode* pools[L2_MAX_NCORES+1];
    struct listnode* shared_pools[L2_MAX_NCORES+1];
    struct listnode* shared_pools_excl[L2_MAX_NCORES+1];
    bool irregular;
};

extern const struct placeinfo * __main_placeinfo;
extern sl_place_t __main_place_id;

static
size_t fast_log2(size_t ncores) {
    ncores |= 1;
    ncores -= 1;
    size_t l2 = __builtin_clzl(ncores);
    l2 = sizeof(size_t)*CHAR_BIT - l2 - 1;
    return (l2 < L2_MAX_NCORES) ? l2 : L2_MAX_NCORES ;
}

static
struct listnode *try_get_dontcare(struct listnode **pools, 
                                  bool remove_from_pool,
                                  size_t lp)
{
    size_t i;
    struct listnode *p;
    for (i = 0; i <= lp; ++i) {
        p = pools[i]; 
        if (p) {
            if (likely(remove_from_pool)) pools[i] = p->next;
            return p;
        }
    }
    return 0;
}

static
struct listnode *try_get_exact(struct listnode **pools, 
                               bool remove_from_pool,
                               size_t lp,
                               size_t l2_pa,
                               size_t ncores,
                               bool irregular)
{
    struct listnode **lprev;
    struct listnode *p;

    if (likely(l2_pa <= lp)) {
        if (likely(!irregular)) {
            if (unlikely(ncores != (1 << l2_pa))) 
                return 0;
            p = pools[l2_pa];
            if (likely(remove_from_pool)) { 
                pools[l2_pa] = p->next;
            }
            return p;
        } else {
            for (lprev = &pools[l2_pa], p = pools[l2_pa]; p; lprev = &p->next, p = p->next)
                if (p->pi.ncores == ncores) {
                    if (likely(remove_from_pool)) { *lprev = p->next; }
                    return p;
                }
        }
    }
    return 0;
}

static
struct listnode *try_get_min(struct listnode **pools, 
                             bool remove_from_pool,
                             size_t lp,
                             size_t l2_pa,
                             size_t ncores,
                             bool irregular)
{
    struct listnode *p;
    struct listnode **lprev;
    size_t i;

    if (likely(!irregular)) {
        if (unlikely(ncores != (1 << l2_pa))) ++l2_pa;

        for (i = l2_pa; i <= lp; ++i) {
            p = pools[i]; 
            if (p) {
                if (likely(remove_from_pool)) { pools[i] = p->next; }
                return p;
            }
        }
    } else {
        for (i = l2_pa; i <= lp; ++i)
            for (lprev = &pools[i], p = pools[i]; p; lprev = &p->next, p = p->next)
                if (p->pi.ncores >= ncores)
                {
                    if (likely(remove_from_pool)) { *lprev = p->next; }
                    return p;
                }
    }
    return 0;
}

static
struct listnode *try_get_max(struct listnode **pools, 
                             bool remove_from_pool,
                             size_t lp,
                             size_t l2_pa,
                             size_t ncores,
                             bool irregular)
{
    struct listnode **lprev;
    struct listnode *p;
    size_t i;

    if (likely(l2_pa <= lp)) {
        i = l2_pa;
        if (likely(!irregular)) {
            do {
                p = pools[i]; 
                if (p) {
                    if (likely(remove_from_pool)) { pools[i] = p->next; }
                    return p;
                }
            } while(i--);
        }
        else {
            do {
                for (lprev = &pools[i], p = pools[i]; p; lprev = &p->next, p = p->next)
                    if (p->pi.ncores <= ncores) {
                        if (likely(remove_from_pool)) { *lprev = p->next; }
                        return p;
                    }
            } while(i--);
        }
    }
    return 0;
}

static
struct listnode* try_get_policy(struct listnode **pools, 
                                bool remove_from_pool,
                                size_t lp,
                                size_t l2_pa,
                                size_t ncores,
                                bool irregular,
                                enum sep_alloc_policy policy)
{
    struct listnode *p = 0;
    switch(policy) {
    case SAL_EXACT:
        p = try_get_exact(pools, remove_from_pool, lp, l2_pa, ncores, irregular);
        break;
    case SAL_MIN:
        p = try_get_min(pools, remove_from_pool, lp, l2_pa, ncores, irregular);
        break;
    case SAL_MAX:
        p = try_get_max(pools, remove_from_pool, lp, l2_pa, ncores, irregular);
        break;
    }
    return p;
}

static
void makeexcl(struct listnode *p)
{
    if (!p->pi.exclusive) {
        p->pi.pid = p->pid | 8 /* suspend */ | 6 /* delegate */ | 1 /* exclusive */;
        p->pi.soft_pid = 0;
        p->pi.exclusive = true;
    }
}

static
void makenormal(struct listnode *p)
{
    p->pi.pid = p->pid | 8 /* suspend */ | 6 /* delegate */;
    p->pi.soft_pid = p->pid | 6 /* delegate */;
    p->pi.exclusive = false;
}

static
void makeshared(struct listnode **pools, struct listnode *p)
{
    if (!p->pi.shared) {
        p->pi.shared = true;
        p->next = pools[p->l2_ncores];
        pools[p->l2_ncores] = p;
    }
}

static
struct listnode *alloc_policy(struct sep_data_t *sd,
                              enum sep_alloc_policy policy,
                              size_t lp,
                              size_t l2_pa,
                              size_t ncores,
                              bool irregular,
                              bool exclusive, bool canshare)
{
    struct listnode *p = 0;
    if (unlikely(exclusive)) {
        if (likely(canshare)) {
            p = try_get_policy(sd->shared_pools_excl, false, lp, l2_pa, ncores, irregular, policy);
            if (!p) p = try_get_policy(sd->pools, true, lp, l2_pa, ncores, irregular, policy);
            if (likely(p)) { 
                makeexcl(p);
                makeshared(sd->shared_pools_excl, p);
            }
        }
        else {
            p = try_get_policy(sd->pools, true, lp, l2_pa, ncores, irregular, policy);
            if (likely(p)) { 
                makeexcl(p);
            }
        }
    } 
    else {
        if (likely(canshare)) {
            p = try_get_policy(sd->shared_pools, false, lp, l2_pa, ncores, irregular, policy);
            if (!p) p = try_get_policy(sd->pools, true, lp, l2_pa, ncores, irregular, policy);
            if (likely(p)) { 
                makenormal(p);
                makeshared(sd->shared_pools, p);
            }
        }
        else {
            p = try_get_policy(sd->pools, true, lp, l2_pa, ncores, irregular, policy);
            if (likely(p)) { 
                makenormal(p);
            }
        }
    }
    return p;
}                              

static
struct listnode *alloc_dontcare(struct sep_data_t *sd,
                                size_t lp,
                                bool exclusive, 
                                bool canshare)
{
    struct listnode *p = 0;
    if (unlikely(exclusive)) {
        if (likely(canshare)) {
            p = try_get_dontcare(sd->shared_pools_excl, false, lp);
            if (!p) p = try_get_dontcare(sd->pools, true, lp);
            if (likely(p)) { 
                makeexcl(p);
                makeshared(sd->shared_pools_excl, p);
            }
        }
        else {
            p = try_get_dontcare(sd->pools, true, lp);
            if (likely(p)) { 
                makeexcl(p);
            }
        }
    } 
    else {
        if (likely(canshare)) {
            p = try_get_dontcare(sd->shared_pools, false, lp);
            if (!p) p = try_get_dontcare(sd->pools, true, lp);
            if (likely(p)) { 
                makenormal(p);
                makeshared(sd->shared_pools, p);
            }
        }
        else {
            p = try_get_dontcare(sd->pools, true, lp);
            if (likely(p)) { 
                makenormal(p);
            }
        }
    }
    return p;
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
    size_t policy = sl_getp(policy) & SAL_EXACT;
    size_t ncores = sl_getp(policy) & 0x3ff;
    bool canshare = sl_getp(policy) & SAL_SHARED;
    bool exclusive = sl_getp(policy) & SAL_EXCLUSIVE;

    size_t lp = sd->last_nonempty_pool;
    if (policy == SAL_DONTCARE) {
        p = alloc_dontcare(sd, lp, exclusive, canshare);
    } else {
        size_t l2_pa = fast_log2(ncores);
        bool irregular = sd->irregular;
        p = alloc_policy(sd, policy, lp, l2_pa, ncores, irregular, exclusive, canshare);
    }

    if (likely(p)) {
        ++(p->sharecount);
        sl_setp(result, &p->pi);
    }
    else
        sl_setp(result, 0);
}
sl_enddef

sl_def(sep_free, void, 
       sl_glparm(struct SEP*, sep),
       sl_glparm(struct placeinfo*, p));
{
    struct sep_data_t* sd = (struct sep_data_t*)(void*)sl_getp(sep);
    struct listnode* n = (struct listnode*)(void*)sl_getp(p);

    size_t l2_ncores = n->l2_ncores;
    --(n->sharecount);
    if (n->sharecount == 0)
    {
        if (n->pi.shared)
        {
            // was shared, need to remove from shared pool
            struct listnode **spools = n->pi.exclusive ? sd->shared_pools_excl : sd->shared_pools;
            struct listnode **prev;
            struct listnode *p;
            for (prev = &spools[n->l2_ncores], p = spools[n->l2_ncores]; 
                 p; prev = &p->next, p = p->next)
                if (p == n) { *prev = p->next; break; }
        }

        n->pi.shared = false;
        n->pi.exclusive = false;
        n->next = sd->pools[l2_ncores];
        sd->pools[l2_ncores] = n; 
    }
}
sl_enddef

sl_def(sep_dump_info, void,
       sl_glparm(struct SEP*, sep))
{
    int i;
    struct sep_data_t* sd = (struct sep_data_t*)(void*)sl_getp(sep);
    printf("loc\t#cores\tshared\texcl\t#users\tpid\tspid\n");
    for (i = 0; i < MAX_NCORES; ++i) {
        struct listnode *p = &sd->allplaces[i];
        if (p->sharecount) {
            printf("%#lx\t%zu\t%d\t%d\t%zu\t%#lx\t%#lx\n",
                   p->pid, p->pi.ncores, p->pi.shared, p->pi.exclusive, p->sharecount,
                   p->pi.pid, p->pi.soft_pid);
        }
    }
}
sl_enddef

static struct sep_data_t root_sep_data = { 
    { 0, 
      &sep_alloc,
      &sep_free,
      &sep_dump_info, },
    0,
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    0
};

struct SEP *root_sep = &root_sep_data.sep_info;

extern int verbose_boot;

void sys_sep_init(void* init_parameters)
{
    struct placeconf {
        uint32_t   ncores;
        uint16_t core_info[][2];
    };
    struct placeconf * restrict pc = (struct placeconf*) init_parameters;

    if (verbose_boot) {
        output_string("* SEP init: parsing layout for ", 2);
        output_uint(pc->ncores, 2);
        output_string(" cores...", 2);
    }
    // can't use assert() before places have been defined
    if (!(pc->ncores <= MAX_NCORES)) {
        output_string("SEP init fail: ncores > MAX_NCORES\n", 2);
        abort();
    }

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
                size_t l2_nc = fast_log2(ncores);
                n->l2_ncores = l2_nc;
                n->next = root_sep_data.pools[l2_nc];
                root_sep_data.pools[l2_nc] = n;
                if (ncores != (1 << l2_nc)) irregular = 1;
                if (max_l2 < l2_nc) max_l2 = l2_nc;
            }

            /* restart a new ring */
            current_ring_id = pc->core_info[i][1];

            /* start configuring the place */
            sl_place_t the_pid = (pc->core_info[i][0] << 4) /* core id */;

            root_sep_data.allplaces[current_ring_id].pid = the_pid;

            root_sep_data.allplaces[current_ring_id].pi.ncores = 1;
            root_sep_data.allplaces[current_ring_id].pi.nfamilies_per_core = *mgconf_ftes_per_core;
            root_sep_data.allplaces[current_ring_id].pi.nthreads_per_core = *mgconf_ttes_per_core;

            if (!root_sep_data.sep_info.sep_place) {
                // first time here
                struct listnode *p = &root_sep_data.allplaces[current_ring_id];

                p->sharecount = 1;
                p->pi.pid = p->pid|8|6;
                p->pi.soft_pid = p->soft_pid|6;
                p->pi.shared = true;
                p->pi.exclusive = false;
                
                // make shared: remove from normal pool, add to shared pool
                root_sep_data.pools[p->l2_ncores] = p->next;
                root_sep_data.shared_pools[p->l2_ncores] = p;
                    
                // share the first place with the SEP.
                root_sep_data.sep_info.sep_place = p->pi.pid|8|6|1;
                __main_place_id = p->pi.pid;
                __main_placeinfo = &p->pi;
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
    size_t l2_nc = fast_log2(ncores);
    n->l2_ncores = l2_nc;
    n->next = root_sep_data.pools[l2_nc];
    root_sep_data.pools[l2_nc] = n;
    if (ncores != (1 << l2_nc)) irregular = 1;
    if (max_l2 < l2_nc) max_l2 = l2_nc;

    root_sep_data.last_nonempty_pool = max_l2;
    root_sep_data.irregular = irregular;

    if (verbose_boot) {
        output_string(" done.\n  places:", 2);
        for (i = 0; i < MAX_NCORES; ++i)
            if (root_sep_data.allplaces[i].pi.pid) {
                output_char(' ', 2);
                output_uint(root_sep_data.allplaces[i].pi.ncores, 2);
                output_char('(', 2);
                output_hex(root_sep_data.allplaces[i].pi.pid, 2);
                output_char(')', 2);
            }
        output_char('\n', 2);
    }
}
