extern unsigned _edata;
extern unsigned _data_start;
extern unsigned _endtext;
extern unsigned _RAM_START;

void __copyram(void) {
    unsigned *src = &_endtext;
    unsigned *dst = &_data_start;
    if (src < &_RAM_START)
	while (dst != &_edata)
	    *dst++ = *src++;
}
