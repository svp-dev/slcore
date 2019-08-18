#include <svp/testoutput.h>

extern char _edata;
extern char _data_start;
extern char _endtext;
extern char _RAM_START;
extern char _bss_start;
extern char _bss_end;

void __copyram(void) {
    unsigned *src = (unsigned*)(void*)&_endtext;
    unsigned *dst = (unsigned*)(void*)&_data_start;
    if (src < &_RAM_START) {
	while (dst != &_edata) {
	    unsigned d = *src++;
	    *dst++ = d;
	}
    }
    bzero(&_bss_start, &_bss_end-&_bss_start);
}
