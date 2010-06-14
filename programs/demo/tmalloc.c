#include <svp/perf.h>
#include <svp/testoutput.h>
#include <svp/slr.h>
#include <assert.h>
#include <stdlib.h>

#define MAXP 200000

void* pointers[MAXP];

sl_def(doalloc, void)
{
    sl_index(i);
    int s = i & 511;
    pointers[i] = malloc((s >> 6) | (s & 0x28) | ((s & 0x7) << 6));
//    output_hex(pointers[i], 1);
//    output_char('\n', 1);
}
sl_enddef

sl_def(dofree, void)
{
    sl_index(i);
//    output_hex(pointers[i], 1);
//    output_char('\n', 1);
    free(pointers[i]);
}
sl_enddef

slr_decl(slr_var(int, mallinfo, "set to print malloc info between alloc and free"),
         slr_var(size_t, N, "number of alloc/free pairs to run"),
         slr_var(long, B,   "block size for creates"));

#ifdef __mt_freestanding__
extern void tls_malloc_stats(void);
#endif


// XIGNORE: *:D

sl_def(t_main, void)
{
    struct s_interval a[2];
    size_t N = 1000;
    if (slr_len(N)) N = slr_get(N)[0];

    assert(N <= MAXP);

    long B = 0;
    if (slr_len(B)) B = slr_get(B)[0];

    int mallinfo = 0;
    if (slr_len(mallinfo)) mallinfo = 1;

    mtperf_start_interval(a, 0, 0, "alloc");
    sl_create(,,,N,,B,,doalloc); sl_sync();
    mtperf_finish_interval(a, 0);

#ifdef __mt_freestanding__
    if (mallinfo) tls_malloc_stats();
#endif

    mtperf_start_interval(a, 1, 0, "free");
    sl_create(,,,N,,B,,dofree); sl_sync();
    mtperf_finish_interval(a, 1);

    mtperf_report_intervals(a, 2, REPORT_CSV|CSV_SEP('\t')|CSV_INCLUDE_HEADER);
}
sl_enddef
