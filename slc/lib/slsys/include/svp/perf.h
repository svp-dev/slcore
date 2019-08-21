//
// perf.h: this file is part of the SL toolchain.
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

#ifndef SLC_SVP_PERF_H
# define SLC_SVP_PERF_H

#include <stddef.h>

/**************** low-level interface ***************/

typedef long counter_t;

#if defined(__mt_freestanding__)

#if defined(__slc_os_sim__)
#define MTPERF_CLOCKS 0
#define MTPERF_EXECUTED_INSNS 1
#define MTPERF_ISSUED_FP_INSNS 2
#define MTPERF_COMPLETED_LOAD_INSNS 3
#define MTPERF_COMPLETED_STORE_INSNS 4
#define MTPERF_CORE_LOADED_BYTES 5
#define MTPERF_CORE_STORED_BYTES 6
#define MTPERF_L2_LOADS 7
#define MTPERF_L2_STORES 8
#define MTPERF_PLACE_SIZE 9
#define MTPERF_CUMUL_TT_OCCUPANCY 10
#define MTPERF_CUMUL_FT_OCCUPANCY 11
#define MTPERF_CUMUL_ALLOC_EX_QSIZE 12
#define MTPERF_UNIX_CTIME 13
#define MTPERF_UNIX_PACKED_DATE 14
#define MTPERF_UNIX_PACKED_TIME 15
#define MTPERF_EXTMEM_LOADS 16
#define MTPERF_EXTMEM_STORES 17
#define MTPERF_CREATED_THREADS 18
#define MTPERF_CREATED_FAMILIES 19
#define MTPERF_CORE_CYCLES 20
#define MTPERF_NCOUNTERS 21
#endif

#if defined(__slc_arch_leon2mt__) || defined(__slc_arch_leon2__)
#define MTPERF_CLOCKS         0
#define MTPERF_EXECUTED_INSNS 1
#define MTPERF_ANNULED_INSNS  2
#define MTPERF_STALLED_INSNS  3
#define MTPERF_SW_ACTIVE_AQ   4
#define MTPERF_SW_WAITING_WQ  5
#define MTPERF_SW_TT_REG_PENDING 6
#define MTPERF_NCOUNTERS      7

#elif defined(__slc_os_fpga__) && defined(__slc_arch_mtsparc__)
#define MTPERF_CLOCKS       0
#define MTPERF_IC_HOLDN     1
#define MTPERF_DC_HOLDN     2
#define MTPERF_FP_HOLDN     3
#define MTPERF_FORCE_NOP    4
#define MTPERF_HOLDN        5
#define MTPERF_IU_SCH_BUS   6
#define MTPERF_SCH_IU_BUS   7
#define MTPERF_EXECUTED_INSNS   8
#define MTPERF_IC_GRANT     9
#define MTPERF_DC_GRANT    10
#define MTPERF_RF_SYN_WR   11
#define MTPERF_RF_ASYN_WR  12
#define MTPERF_RF_CONC_WR  13
#define MTPERF_RAU_RLS     14
#define MTPERF_RAU_ALLOC   15
#define MTPERF_DC_MISS     16
#define MTPERF_S_HOLDN     17
#define MTPERF_RF_HOLDN    18
#define MTPERF_NCOUNTERS   19
#endif

#else

#define MTPERF_CLOCKS 0
#define MTPERF_UNIX_TIME 1
#define MTPERF_NCOUNTERS 2

#endif

// get the current value for the specified counter
counter_t mtperf_sample1(int thecounter);

// sample all counters into an array (must be large enough for MTPERF_NCOUNTERS values)
void mtperf_sample(counter_t* where);

// print out the pair-wise differences between counters using the specified
// flags and format (see below)
void mtperf_report_diffs(const counter_t *before, const counter_t* after, int flags);

// formats for report_diffs
#define CSV_SEP(C) ((((int)(C))<<16)|2)
#define CSV_INCLUDE_HEADER 1
#define REPORT_NOLF 4
#define REPORT_CSV 0
#define REPORT_FIBRE (2<<8)
#define REPORT_RAW (1<<8)
#define REPORT_STREAM(N) ((N)<<24)
#define FIBRE_PAD(N) (((int)(N))<<16)


/**************** higher-level interface ***************/

// A structure for a named interval.
struct s_interval {
  counter_t before[MTPERF_NCOUNTERS];
  counter_t after[MTPERF_NCOUNTERS];
  const char *tag;
  int num;
  unsigned magic;
  unsigned ncounters;
};

#define _PERF_MAGIC_NUMBER_ 0xCAFEF00D

// allocate multiple intervals.
struct s_interval* mtperf_alloc_intervals(size_t n);

// free an array of intervals.
void mtperf_free_intervals(struct s_interval* p);

// clear the p-th interval in the array
// This define the interval as if the start and finish events were simultaneous.
void mtperf_empty_interval(struct s_interval* ivs, size_t p,
			   int numarg, const char *tag);

// sample the start event for the p-th interval in array ivs
void mtperf_start_interval(struct s_interval* ivs, size_t p,
			   int numarg,
			   const char *tag);

// sample the finish event for the p-th interval in array ivs
void mtperf_finish_interval(struct s_interval* ivs, size_t p);

// report the data for all intervals.
// this uses the same flags and format as mtperf_report_diffs above.
void mtperf_report_intervals(const struct s_interval* ivs, size_t n, int flags);


/**************** inlinable primitives **************/

#if !defined(_AVOID_INLINABLE_PRIMITIVES)

#include <stdlib.h>
#include <svp/compiler.h>
#include <assert.h>

alwaysinline unused
struct s_interval* __inline_mtperf_alloc_intervals(size_t n)
{
    return (struct s_interval*)calloc(n, sizeof(struct s_interval));
}
#define mtperf_alloc_intervals(N) __inline_mtperf_alloc_intervals(N)

alwaysinline unused
void __inline_mtperf_free_intervals(struct s_interval* p)
{
    free(p);
}
#define mtperf_free_intervals(P) __inline_mtperf_free_intervals(P)


#if defined(__mt_freestanding__)

#if defined(__slc_os_fpga__) || defined(__slc_os_tbdef__) // && defined(__slc_arch_mtsparc__)
// AppleCORE performance counters on uT-LEON3, as of April 2011
#define __MTPERF_CT_BASE 0x80000810
#endif
#ifdef __slc_os_sim__
// Microgrid base address for perf counters
#define __MTPERF_CT_BASE 8
#endif

typedef struct { counter_t ct[MTPERF_NCOUNTERS]; } __counters_t;

#if defined(__GNUC__) && !defined(__AVOID_GNUISMS)

alwaysinline unused
void __inline_mtperf_sample(counter_t * array)
{
        __asm__ __volatile__("");
        __counters_t* restrict __dst = (__counters_t*)(void*)(array);
#if false // defined(__slc_arch_leon2__) || defined(__slc_arch_leon2mt__)
#define __read_cnt(i)                                                   \
    if ((i + 1) <= MTPERF_NCOUNTERS) {					\
	counter_t __ct ## i;						\
	__asm__ __volatile__("rd %%wim, %1, %0" : "=r"(__ct ## i) : "I"(i)); \
	__dst->ct[i] = __ct ## i;					\
    }
#else
        volatile __counters_t* restrict __src = (volatile __counters_t*)(void*)__MTPERF_CT_BASE;
#define __read_cnt(i)                                                   \
        if ((i + 1) <= MTPERF_NCOUNTERS) {                              \
            counter_t __ct ## i;                                        \
            __asm__("" : "=r"(__ct ## i) : "0"(__src->ct[i])); \
            __dst->ct[i] = __ct ## i;                                   \
        }
#endif
	
        __read_cnt(0);
        __read_cnt(1);
        __read_cnt(2);
        __read_cnt(3);
        __read_cnt(4);
        __read_cnt(5);
        __read_cnt(6);
        __read_cnt(7);
        __read_cnt(8);
        __read_cnt(9);
        __read_cnt(10);
        __read_cnt(11);
        __read_cnt(12);
        __read_cnt(13);
        __read_cnt(14);
        __read_cnt(15);
        __read_cnt(16);
        __read_cnt(17);
        __read_cnt(18);
        __read_cnt(19);
        __read_cnt(20);
#undef __read_cnt
        __asm__ __volatile__("");
}
#define mtperf_sample(Array) __inline_mtperf_sample(Array)

#endif /* __GNUC__ && !__AVOID_GNUISMS */

alwaysinline unused
counter_t __inline_mtperf_sample1(int counter)
{
#if false // defined(__slc_arch_leon2__) || defined(__slc_arch_leon2mt__)
    counter_t ct;
    switch(counter) {
#define rcnt(ci) case ci:  __asm__ __volatile__("rd %%wim, %1, %0" : "=r"(ct) : "I"(ci)); break
    rcnt(0);
    rcnt(1);
    rcnt(2);
    rcnt(3);
    rcnt(4);
    rcnt(5);
    rcnt(6);
#undef rcnt
    default: svp_abort();
}
    return ct;
#else
    return ((volatile __counters_t*restrict)(void*)__MTPERF_CT_BASE)->ct[counter];
#endif
}
#define mtperf_sample1(Counter) __inline_mtperf_sample1(Counter)

#else /* !__mt_freestanding__ */

#include <time.h>

alwaysinline unused
void __inline_mtperf_sample(counter_t* array)
{
    array[0] = (counter_t)clock();
    array[1] = (counter_t)time(0);
}
#define mtperf_sample(Array) __inline_mtperf_sample(Array)

alwaysinline unused
counter_t __inline_mtperf_sample1(int counter)
{
    counter_t val;
    switch(counter) {
    case 0: val = clock(); break;
    case 1: val = time(0); break;
    default: val = 0; break;
    }
    return val;
}
#define mtperf_sample1(Counter) __inline_mtperf_sample1(Counter)

#endif /* !__mt_freestanding__ */

alwaysinline unused
void __inline_mtperf_empty_interval(struct s_interval *ivs, size_t p,
                                    int numarg, const char *tag)
{
    struct s_interval init = { { 0 }, { 0 }, tag, numarg, _PERF_MAGIC_NUMBER_, MTPERF_NCOUNTERS };
    ivs[p] = init;
}

#define mtperf_empty_interval(Ivs, P, Numarg, Tag)      \
    __inline_mtperf_empty_interval(Ivs, P, Numarg, Tag)

alwaysinline unused
void __inline_mtperf_start_interval(struct s_interval *ivs, size_t p,
                                    int numarg, const char *tag)
{
    ivs[p].tag = tag;
    ivs[p].num = numarg;
    ivs[p].magic = _PERF_MAGIC_NUMBER_;
    ivs[p].ncounters = MTPERF_NCOUNTERS;
    mtperf_sample(ivs[p].before);
}
#define mtperf_start_interval(Ivs, P, Numarg, Tag) \
    __inline_mtperf_start_interval(Ivs, P, Numarg, Tag)

alwaysinline unused
void __inline_mtperf_finish_interval(struct s_interval *ivs, size_t p)
{
    mtperf_sample(ivs[p].after);
    assert(ivs[p].magic == _PERF_MAGIC_NUMBER_);
    assert(ivs[p].ncounters == MTPERF_NCOUNTERS);
}
#define mtperf_finish_interval(Ivs, P) __inline_mtperf_finish_interval(Ivs, P)

#endif /* !defined(__AVOID_INLINABLE_PRIMITIVES) */

#endif // ! SLC_SVP_PERF_H
