#include <svp/testoutput.h>

extern unsigned _edata;
extern unsigned _data_start;
extern unsigned _endtext;
extern unsigned _RAM_START;
extern unsigned _bss_start;
extern unsigned _bss_end;

void __copyram(void) {
    unsigned *src = &_endtext;
    unsigned *dst = &_data_start;
    if (src < &_RAM_START) {
	while (dst != &_edata) {
	    unsigned d = *src++;
	    *dst++ = d;
	}
    }
    bzero(&_bss_start, &_bss_end-&_bss_start);
}
