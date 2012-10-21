//
// perf.c: this file is part of the SL toolchain.
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

#include <assert.h>
#include <svp/perf.h>
#include <svp/testoutput.h>

#if defined(__slc_os_sim__)
#include "mgsim/mtconf.h"
#endif

#include <stdio.h>

// the ordering in the following
// table should match the counter
// indices in perf.h.
const char* mtperf_counter_names[] = {
    "clocks",
#if defined(__mt_freestanding__)
#if defined(__slc_os_sim__)
    "n_exec_insns",
    "n_issued_flops",
    "n_compl_core_loads",
    "n_compl_core_stores",
    "n_bytes_l1tocore",
    "n_bytes_coretol1",
    "n_reqs_l2tol1",
    "n_reqs_l1tol2",
    "notused1",
    "tt_total",
    "ft_total",
    "xq_total",
    "notused2",
    "notused3",
    "notused4",
    "n_cl_loads_ext",
    "n_cl_stores_ext",
    "n_cr_threads",
    "n_cr_families",
    "core_clocks",
// computed columns
    "pl_eff",
    "tt_occp",
    "ft_occp",
    "xq_avg",
#endif
#if defined(__slc_os_fpga__)
    "ic_holdn",
    "dc_holdn",
    "fp_holdn",
    "force_nop",
    "holdn",
    "iu_sch_bus",
    "sch_iu_bus",
    "committed",
    "ic_grant",
    "dc_grant",
    "rf_syn_wr",
    "rf_asyn_wr",
    "rf_conc_wr",
    "rau_rls",
    "rau_alloc",
    "dc_miss",
    "s_holdn",
    "rf_holdn",
// computed columns
    "pl_eff_x100",
#endif
#else
    "unixtime",
#endif
};

#define pc(Ch) output_char((Ch), stream)
#define ps(Str) output_string((Str), stream)
#define pn(Num) output_int((Num), stream)
#define pnl  output_char('\n', stream);

#if defined(__mt_freestanding__) && defined(__slc_os_fpga__)
#define ARITH long
#define pf(Num) output_int((Num), stream)
#else
#define ARITH float
#define pf(Num) output_float((Num), stream, 6)
#endif



#define bfibre(N) do {                          \
        output_char('[', stream);               \
        output_char('1', stream);               \
        output_char(',', stream);               \
        output_int((N), stream);                \
        output_char(':', stream);               \
    } while(0)
#define efibre output_char(']', stream)

#define pnlsep pc((fflags & REPORT_NOLF) ? ' ' : '\n');

#ifdef max
#undef max
#endif
#define max(a,b) ((a) > (b) ? (a) : (b))


static 
ARITH mtperf_compute_extra(const counter_t* before, const counter_t* after, unsigned extra)
{
#if defined(__mt_freestanding__)
#if defined(__slc_os_sim__)
    counter_t ncores = after[MTPERF_PLACE_SIZE];
    long core_rate = mgconf_master_freq / mgconf_core_freq;
#else
    counter_t ncores = 1;
    long core_rate = 100;
#endif

    counter_t elapsed = after[MTPERF_CLOCKS] - before[MTPERF_CLOCKS];

    switch (extra) {
    case 0:
    {
        ARITH itotal = after[MTPERF_EXECUTED_INSNS] - before[MTPERF_EXECUTED_INSNS];
        itotal /= ncores;
        if (elapsed) return itotal * core_rate / elapsed;
        break;
    }
#if defined(__slc_os_sim__)
    case 1:
    {
        // thread table occupancy
        ARITH ttotal = after[MTPERF_CUMUL_TT_OCCUPANCY] - before[MTPERF_CUMUL_TT_OCCUPANCY];
        ttotal /= ncores;
        ttotal /= mgconf_ttes_per_core;
        if (elapsed) return ttotal /= elapsed;
        break;
    }
    case 2:
    {
        // family table occupancy
        ARITH ttotal = after[MTPERF_CUMUL_FT_OCCUPANCY] - before[MTPERF_CUMUL_FT_OCCUPANCY];
        ttotal /= ncores;
        ttotal /= mgconf_ftes_per_core;
        if (elapsed) return ttotal /= elapsed;
        break;
    }
    case 3:
    {
        // exclusive allocate queue size
        ARITH ttotal = after[MTPERF_CUMUL_ALLOC_EX_QSIZE] - before[MTPERF_CUMUL_ALLOC_EX_QSIZE];
        ttotal /= ncores;
        if (elapsed) return ttotal /= elapsed;
        break;
    }
#define N_EXTRA_COLUMNS 4
#else
#define N_EXTRA_COLUMNS 1
#endif
    }
#else
#define N_EXTRA_COLUMNS 0
#endif

    return 0;        
}

void mtperf_report_diffs(const counter_t* before, const counter_t* after, int flags)
{
    int i;

    int stream = (flags >> 24) & 0xff;
    if (!stream) stream = 1;
    int format = (flags >> 8) & 0xff;
    int fflags = flags & 0xff;
    switch (format) {
    case 0:
    {
        // output CSV
        int print_headers = fflags & 1;
        int spec_sep = fflags & 2;
        char sep = spec_sep ? ((flags >> 16) & 0xff) : ',';
        if (print_headers) {
            // print column headers
            for (i = 0; i < MTPERF_NCOUNTERS + N_EXTRA_COLUMNS; ++i) {
                if (i) pc(sep);
                ps(mtperf_counter_names[i]);
            }
            pnl;
        }
        for (i = 0; i < MTPERF_NCOUNTERS; ++i) {
            if (i) pc(sep);
            pn(after[i]-before[i]);
        }
        for (i = 0; i < N_EXTRA_COLUMNS; ++i) {
            pc(sep);
            pf(mtperf_compute_extra(before, after, i));
        }
        pnlsep;
    }
    break;
    case 1:
        // output raw
        for (i = 0; i < MTPERF_NCOUNTERS; ++i) {
            pn(after[i]-before[i]);
            pnlsep;
        }
        for (i = 0; i < N_EXTRA_COLUMNS; ++i) {
            pf(mtperf_compute_extra(before, after, i));
            pnlsep;
        }
        break;
    case 2:
        // output Fibre
    {
        int pad = (flags >> 16) & 0xff;
        bfibre(max(MTPERF_NCOUNTERS+N_EXTRA_COLUMNS, pad));
        for (i = 0; i < MTPERF_NCOUNTERS; ++i) {
            pc(' ');
            pn(after[i]-before[i]);
        }
        for (i = 0; i < N_EXTRA_COLUMNS; ++i) {
            pc(' ');
            pf(mtperf_compute_extra(before, after, i));
        }
        for ( ; i < pad; ++i) ps(" 0");
        efibre;
        pnlsep;
    }
    break;
    }
}

void mtperf_report_intervals(const struct s_interval* ivs,
			     size_t n,
			     int flags)
{
    int i, j;

    int stream = (flags >> 24) & 0xff;
    if (!stream) stream = 1;
    int format = (flags >> 8) & 0xff;
    int fflags = flags & 0xff;

    switch (format) {
    case 0:
    {
        // output CSV
        int print_headers = fflags & 1;
        int spec_sep = fflags & 2;
        char sep = spec_sep ? ((flags >> 16) & 0xff) : ',';
        if (print_headers) {
            // print column headers
            for (i = 0; i < MTPERF_NCOUNTERS+N_EXTRA_COLUMNS; ++i) {
                if (i) pc(sep);
                pc('"');
                ps(mtperf_counter_names[i]);
                pc('"');
            }
            if (i) pc(sep);
            ps("\"intervals\"");
            pnl;
        }
        for (j = 0; j < n; ++j) {
            assert(ivs[j].magic == _PERF_MAGIC_NUMBER_);
            assert(ivs[j].ncounters == MTPERF_NCOUNTERS);

            for (i = 0; i < MTPERF_NCOUNTERS; ++i) {
                if (i) pc(sep);
                pn(ivs[j].after[i]-ivs[j].before[i]);
            }
            for (i = 0; i < N_EXTRA_COLUMNS; ++i) {
                pc(sep);
                pf(mtperf_compute_extra(ivs[j].before, ivs[j].after, i));
            }                
            if (print_headers) {
                pc(sep);
                pc('"');
                if (ivs[j].num >= 0) { pn(ivs[j].num); pc(' '); }
                ps(ivs[j].tag ? ivs[j].tag : "(anon)");
                pc('"');
            }
            pnlsep;
        }
        ps("\"(mtperf compiled with slc " __slc_version_string__ ")\"");
        pnlsep;
    }
    break;
    case 1:
        // output raw
        ps("# metrics\n");
        for (i = 0; i < MTPERF_NCOUNTERS+N_EXTRA_COLUMNS; ++i) {
            ps(mtperf_counter_names[i]);
            pnlsep;
        }
        for (j = 0; j < n; ++j) {
            assert(ivs[j].magic == _PERF_MAGIC_NUMBER_);
            assert(ivs[j].ncounters == MTPERF_NCOUNTERS);

            ps("# ");
            if (ivs[j].num >= 0) { pn(ivs[j].num); pc(' '); }
            ps(ivs[j].tag ? ivs[j].tag : "(anon)");
            pnl;
            for (i = 0; i < MTPERF_NCOUNTERS; ++i) {
                pn(ivs[j].after[i]-ivs[j].before[i]);
                pnlsep;
            }
            for (i = 0; i < N_EXTRA_COLUMNS; ++i) {
                pf(mtperf_compute_extra(ivs[j].before, ivs[j].after, i));
                pnlsep;
            }         
        }
        ps("# (mtperf compiled with slc " __slc_version_string__ ")");
        pnlsep;
        break;
    case 2:
        // output Fibre
    {
        int pad = ((flags >> 16) & 0xff);
        int dmax = 0;
        if (pad)
            dmax = max(pad, max(n, MTPERF_NCOUNTERS+N_EXTRA_COLUMNS));

        ps("### begin intervals\n");
        bfibre(n); 
        pnlsep;
        for (j = 0; j < n; ++j) {
            assert(ivs[j].magic == _PERF_MAGIC_NUMBER_);
            assert(ivs[j].ncounters == MTPERF_NCOUNTERS);

            bfibre(MTPERF_NCOUNTERS+N_EXTRA_COLUMNS);
            for (i = 0; i < MTPERF_NCOUNTERS; ++i) {
                pc(' '); 
                pn(ivs[j].after[i]-ivs[j].before[i]);
            }
            for (i = 0; i < N_EXTRA_COLUMNS; ++i) {
                pc(' ');
                pf(mtperf_compute_extra(ivs[j].before, ivs[j].after, i));
            }         
            efibre;
            pnlsep;
        }
        efibre;
        pnlsep;
        if (flags & REPORT_NOLF) pnl;
        ps("### end intervals\n### begin descriptions\n");

        bfibre(dmax ? dmax : n);
        for (i = 0; i < n; ++i) {
            pc(' ');
            pc('"');
            if (ivs[i].num >= 0) { pn(ivs[i].num); pc(' '); }
            ps(ivs[i].tag ? ivs[i].tag : "(anon)");
            pc('"');
        }
        for ( ; i < dmax; ++i) ps(" \"\"");
        efibre;
      
        pnlsep;
      
        bfibre(dmax ? dmax : MTPERF_NCOUNTERS+N_EXTRA_COLUMNS);
        for (i = 0; i < MTPERF_NCOUNTERS+N_EXTRA_COLUMNS; ++i) {
            pc(' ');
            pc('"');
            ps(mtperf_counter_names[i]);
            pc('"');
        }
        for ( ; i < dmax; ++i) ps(" \"\"");
        efibre;
      
        ps("\n"
           "### end descriptions\n"
           "# (mtperf compiled with slc " __slc_version_string__ ")\n");
    }
    break;
    }
  
}


