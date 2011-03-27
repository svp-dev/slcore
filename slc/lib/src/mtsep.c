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

struct sep_pi {
    sl_place_t pid;
    size_t     ncores;
};

struct listnode {
    struct sep_pi pi;
    struct listnode *next;
    size_t l2_ncores;
    size_t sharecount;
    sl_place_t pid;
};

struct sep_data_t {
    struct SEP sep_info;  
    sl_place_t sep_pid;
    size_t last_nonempty_pool;
    struct listnode  allplaces[MAX_NCORES];
    struct listnode* pools[L2_MAX_NCORES+1];
    struct listnode* allcores[MAX_NCORES];
    bool irregular;
};

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
                                  size_t lp)
{
    size_t i;
    struct listnode *p;
    for (i = 0; i <= lp; ++i) {
        p = pools[i]; 
        if (p) {
            pools[i] = p->next;
            return p;
        }
    }
    return 0;
}

static
struct listnode *try_get_exact(struct listnode **pools, 
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
            if (likely(p)) pools[l2_pa] = p->next;
            return p;
        } else {
            for (lprev = &pools[l2_pa], p = pools[l2_pa]; p; lprev = &p->next, p = p->next)
                if (p->pi.ncores == ncores) {
                    *lprev = p->next; 
                    return p;
                }
        }
    }
    return 0;
}

static
struct listnode *try_get_min(struct listnode **pools, 
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
                pools[i] = p->next; 
                return p;
            }
        }
    } else {
        for (i = l2_pa; i <= lp; ++i)
            for (lprev = &pools[i], p = pools[i]; p; lprev = &p->next, p = p->next)
                if (p->pi.ncores >= ncores)
                {
                    *lprev = p->next;
                    return p;
                }
    }
    return 0;
}

static
struct listnode *try_get_max(struct listnode **pools, 
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
                    pools[i] = p->next; 
                    return p;
                }
            } while(i--);
        }
        else {
            do {
                for (lprev = &pools[i], p = pools[i]; p; lprev = &p->next, p = p->next)
                    if (p->pi.ncores <= ncores) {
                        *lprev = p->next; 
                        return p;
                    }
            } while(i--);
        }
    }
    return 0;
}

static
struct listnode* try_get_policy(struct listnode **pools, 
                                size_t lp,
                                size_t l2_pa,
                                size_t ncores,
                                bool irregular,
                                long policy)
{
    struct listnode *p = 0;
    switch(policy) {
    case SAL_EXACT:
        p = try_get_exact(pools, lp, l2_pa, ncores, irregular);
        break;
    case SAL_MIN:
        p = try_get_min(pools,   lp, l2_pa, ncores, irregular);
        break;
    case SAL_MAX:
        p = try_get_max(pools,   lp, l2_pa, ncores, irregular);
        break;
    }
    return p;
}


static
void makenormal(struct listnode *p)
{
    p->pi.pid = p->pid | 8 /* suspend */ | 6 /* delegate */;
}

static
struct listnode *alloc_policy(struct sep_data_t *sd,
                              long policy,
                              size_t lp,
                              size_t l2_pa,
                              size_t ncores,
                              bool irregular)
{
    struct listnode *p = 0;
    p = try_get_policy(sd->pools, lp, l2_pa, ncores, irregular, policy);
    if (likely(p)) { 
        makenormal(p);
    }
    return p;
}                              

static
struct listnode *alloc_dontcare(struct sep_data_t *sd,
                                size_t lp)
{
    struct listnode *p = 0;
    p = try_get_dontcare(sd->pools, lp);
    if (likely(p)) { 
        makenormal(p);
    }
    return p;
}



sl_def(sep_alloc,
       void,
       sl_glparm(struct sep_data_t*, sd),
       sl_glparm(long, policy),
       sl_glparm(size_t, ncores),
       sl_shparm(struct sep_pi*, result))
{
    struct sep_data_t* sd = sl_getp(sd);
    int i;
    struct listnode *p = 0;
    size_t policy = sl_getp(policy);
    size_t ncores = sl_getp(ncores);
    bool canshare = false;

    size_t lp = sd->last_nonempty_pool;
    if (policy == SAL_DONTCARE) {
        p = alloc_dontcare(sd, lp);
    } else {
        size_t l2_pa = fast_log2(ncores);
        bool irregular = sd->irregular;
        p = alloc_policy(sd, policy, lp, l2_pa, ncores, irregular);
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
       sl_glparm(struct sep_data_t*, sd),
       sl_glparm(struct listnode*, n));
{
    struct sep_data_t* sd = sl_getp(sd);
    struct listnode* n = sl_getp(n);

    if (n->sharecount == 0)
        return;

    size_t l2_ncores = n->l2_ncores;
    --(n->sharecount);
    
    n->next = sd->pools[l2_ncores];
    sd->pools[l2_ncores] = n; 
}
sl_enddef

void sep_dump_info(struct SEP* sep)
{
    int i;
    struct sep_data_t* sd = (struct sep_data_t*)(void*)sep;
    printf("loc\t#cores\tpid\n");
    for (i = 0; i < MAX_NCORES; ++i) {
        struct listnode *p = &sd->allplaces[i];
        if (p->sharecount) {
            printf("%#lx\t%zu\t%#lx\n",
                   p->pid, p->pi.ncores, p->pi.pid);
        }
    }
    for (i = 0; i < L2_MAX_NCORES+1; ++i)
    {
        printf("pool %d", i);
        struct listnode *p = sd->pools[i];
        if(!p)
            printf(" (empty)");
        while(p) {
            printf(" -> (%#lx, %zu)", p->pid, p->pi.ncores);
            p = p->next;
        };
        putchar('\n');
    }
}

static
int root_sep_ctl(struct SEP* sep, unsigned long request, void *a, void *b)
{
    struct sep_data_t* sd = (struct sep_data_t*)(void*)sep;
    
    switch(request & 0xffff)
    {
    case SEP_ALLOC:
    {
        sl_create(,sd->sep_pid,,,,,sl__exclusive, sep_alloc,
                  sl_glarg(struct sep_data_t*,, sd),
                  sl_glarg(long,, (request & ~0xffff)),
                  sl_glarg(size_t,, (long)a),
                  sl_sharg(struct sep_pi*, result, 0));
        sl_sync();
        if (sl_geta(result))
        {
            *(sl_place_t*)b = sl_geta(result)->pid;
            return 0;
        }
        else
            return -1;
    }   
                  
    case SEP_FREE:
    {
        sl_place_t p = *(sl_place_t*)a;
        unsigned core = (p >> 4) & 0xffff;
        sl_create(,sd->sep_pid,,,,,sl_exclusive, sep_free,
                  sl_glarg(struct sep_data_t*,, sd),
                  sl_glarg(struct listnode*, , sd->allcores[core]));
        sl_sync();
        return 0;
    }
        
    case SEP_QUERY:
    {
        sl_place_t p = *(sl_place_t*)a;
        union placeinfo *ret = (union placeinfo*)b;
        
        if (p == PLACE_LOCAL)
        {
            ret->flags = 4|2|1; /* hardware, atomic, programmable */
            ret->a.p.family_capacity = *mgconf_ftes_per_core;
            ret->a.p.thread_capacity = *mgconf_ttes_per_core;
            ret->a.p.rate            = *mgconf_core_freq;
        }
        else
        {
            unsigned core = (p >> 4) & 0xffff;
            struct listnode *ln = sd->allcores[core];
            if (!ln)
                return -1;
            ret->flags = 4|1; /* hardware, compound, homogeneous */
            ret->c.arity = ln->pi.ncores;
        }
        return 0;
    }
    default:
        return -1;
    }
}

static struct sep_data_t root_sep_data = { 
    { &root_sep_ctl },
    0,
    0,
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
        output_string(" cores...\n  ", 2);
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
        if (verbose_boot) output_char('.', 2);

        root_sep_data.allcores[i] = &root_sep_data.allplaces[pc->core_info[i][1]];

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
                if (verbose_boot) output_char(' ', 2);
            }

            /* restart a new ring */
            current_ring_id = pc->core_info[i][1];

            /* start configuring the place */
            sl_place_t the_pid = pc->core_info[i][0] << 4;

            struct listnode *current = &root_sep_data.allplaces[current_ring_id];
            current->pid = the_pid;
            current->pi.ncores = 1;

            if (!root_sep_data.sep_pid) {
                // first time here

                current->sharecount = 1;
                current->pi.pid = current->pid|8|6;
                
                // make shared: remove from normal pool, add to shared pool
                // root_sep_data.pools[current->l2_ncores] = current->next;
                    
                root_sep_data.sep_pid = current->pi.pid|8|6|1;
                __main_place_id = current->pi.pid;
                if (verbose_boot) output_char(' ', 2);
            }
        }

        else {
            /* already started the ring, increase the number of cores */
            root_sep_data.allplaces[current_ring_id].pi.ncores += 1;
        }
    }

    struct listnode* n = &root_sep_data.allplaces[current_ring_id];
    size_t ncores = n->pi.ncores;
    size_t l2_nc = fast_log2(ncores);
    if (ncores != (1 << l2_nc)) irregular = 1;
    if (max_l2 < l2_nc) max_l2 = l2_nc;

    /* if more than one ring, add the last item to its pool */
    if (current_ring_id > 0)
    {
        n->l2_ncores = l2_nc;
        n->next = root_sep_data.pools[l2_nc];
        root_sep_data.pools[l2_nc] = n;
    }
    
    root_sep_data.last_nonempty_pool = max_l2;
    root_sep_data.irregular = irregular;

    if (verbose_boot) {
        output_string(" done.\n  places:", 2);
        for (i = 0; i < MAX_NCORES; ++i)
            if (!i || root_sep_data.allplaces[i].pid) {
                output_char(' ', 2);
                output_uint(root_sep_data.allplaces[i].pi.ncores, 2);
                output_char('(', 2);
                output_hex(root_sep_data.allplaces[i].pid, 2);
                output_char(')', 2);
            }
        output_char('\n', 2);
    }
}
