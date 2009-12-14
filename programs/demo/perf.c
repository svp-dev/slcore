#include <svp/perf.h>

sl_def(t_main, void)
{
    counter_t a[MTPERF_NCOUNTERS];
    counter_t b[MTPERF_NCOUNTERS];
    mtperf_sample(b);
    mtperf_sample(a);
    mtperf_report_diffs(b, a, REPORT_CSV | CSV_INCLUDE_HEADER | CSV_SEP(' '));
}
sl_enddef
