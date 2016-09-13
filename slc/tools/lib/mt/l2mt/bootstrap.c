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
	output_char('D', 2);
	output_char('\n', 2);
	while (dst != &_edata) {
	    output_hex(src, 2);
	    output_char('>', 2);
	    output_hex(dst, 2);
	    output_char('=', 2);
	    unsigned d = *src++;
	    output_hex(d, 2);
	    *dst++ = d;
	    output_char('\n', 2);
	}
	output_char('B',2);
	output_char('\n',2);
	output_char('!', 2);
	output_char('\n', 2);
    }
    for (dst = &_bss_start; dst != &_bss_end; ++dst) {
	output_hex(dst, 2);
	output_char('\n', 2);
	*dst = 0;
    }
}
