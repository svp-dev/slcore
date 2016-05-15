#include <svp/testoutput.h>

char __dbgout_bytes;
char __dbgerr_bytes;
volatile int __dbg_exit_status;

char __dbgbuf[__DBGBUF_SIZE];
unsigned long __dbgbuf_p = 0;

const char __dbgfmt_digits[] = "0123456789abcdef";
