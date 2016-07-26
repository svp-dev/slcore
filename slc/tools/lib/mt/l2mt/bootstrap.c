extern unsigned _edata;
extern unsigned _data_start;
extern unsigned _endtext;

void __copyram(void) {
    unsigned *src = &_endtext;
    unsigned *dst = &_data_start;
    while (dst != &_edata)
	*dst++ = *src++;
}
