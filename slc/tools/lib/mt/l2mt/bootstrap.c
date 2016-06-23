extern unsigned _edata;
extern unsigned _data_start;
extern unsigned _etext;

void __copyram(void) {
    unsigned *src = &_etext;
    unsigned *dst = &_data_start;
    while (dst != &_edata)
	*dst++ = *src++;
}
