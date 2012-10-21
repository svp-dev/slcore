#include <svp/testoutput.h>
#include <errno.h>

#include "mgsim/mtconf.h"
#include "stdio/mtstdio.h"

#if defined(__mips__)
#define drain(X) __asm__ __volatile__("move %0, %0" : : "r"(X))
#else
#define drain(X) __asm__ __volatile__("mov %0, %0" : : "r"(X))
#endif

sl_def(do_uart_init, sl__static, sl_glparm(char*,ctl), sl_glparm(long*,chans))
{
    volatile unsigned char* ctl = sl_getp(ctl);
    volatile long* chans = sl_getp(chans);
    // using chans 0/1 for I/O

    // drain channels
    chans[0] = 0; // disable channels
    chans[1] = 0; 
    drain(chans[0]);     drain(chans[1]);  //drain
    drain(chans[0]);     drain(chans[1]); 
    drain(chans[0]);     drain(chans[1]); 
    drain(chans[0]);     drain(chans[1]); 
    drain(chans[0]);     drain(chans[1]); 

    ctl[1] = 3; // enable receive buffer full interrupt / enable transmit buffer empty interrupt
    ctl[8] = 0; // RBFI uses chan 0
    ctl[9] = 1; // TBEI uses chan 1
    ctl[10] = 1; // enable UART

}
sl_enddef

void uart_init(void)
{
    if (mg_uart_devid == -1)
    {
        output_string("uart_init: no UART available\n", 2);
        return;
    }
    
    sl_create(,mg_io_place_id,,,,,, do_uart_init, 
              sl_glarg(char*,, mg_devinfo.base_addrs[mg_uart_devid]),
              sl_glarg(long*,, mg_devinfo.channels)); 
    sl_sync();
}

sl_def(do_uart_deinit, sl__static, sl_glparm(char*,ctl), sl_glparm(long*,chans))
{
    volatile unsigned char* ctl = sl_getp(ctl);
    volatile long* chans = sl_getp(chans);

    // disable UART
    ctl[10] = 0;

    // disable channels
    chans[0] = 0;
    chans[1] = 0;
}
sl_enddef

void uart_deinit(void)
{
    if (mg_uart_devid == -1) return;

    sl_create(,mg_io_place_id,,,,,, do_uart_deinit, 
              sl_glarg(char*,, mg_devinfo.base_addrs[mg_uart_devid]),
              sl_glarg(long*,, mg_devinfo.channels)); 
    sl_sync();
}

sl_def(do_uart_input_byte, sl__static, sl_glparm(char*,ctl), sl_glparm(long*,chans), sl_glparm(unsigned char*, b))
{
    volatile unsigned char* ctl = sl_getp(ctl);
    volatile long* chans = sl_getp(chans);

    chans[0] = 1;
    while(1)
    {
        // wait for RBFI
        drain(chans[0]);

        if (ctl[5] & 1) // data ready?
        {
            // got one byte
            *sl_getp(b) = ctl[0];
            break;
        }
    }
    chans[0] = 0;
}
sl_enddef

sl_def(do_uart_output_byte, sl__static, sl_glparm(char*,ctl), sl_glparm(long*,chans), sl_glparm(char, b))
{
    volatile unsigned char* ctl = sl_getp(ctl);
    volatile long* chans = sl_getp(chans);

    chans[1] = 1;
    while(1)
    {
        // wait for TBEI
        drain(chans[1]);

        if (ctl[5] & (1 << 5)) // THR empty?
        {
            // can send one byte
            ctl[0] = sl_getp(b);
            break;
        }
    }
    chans[1] = 0;
}
sl_enddef

int uart_getc(void* st)
{
    if (mg_uart_devid == -1) { errno = ENXIO; return -1; }
    
    unsigned char b;
    sl_create(,mg_io_place_id,,,,,, do_uart_input_byte, 
              sl_glarg(char*,, mg_devinfo.base_addrs[mg_uart_devid]),
              sl_glarg(long*,, mg_devinfo.channels),
              sl_glarg(unsigned char*,, &b)); 
    sl_sync();
    return b;
}

int uart_putc(char c, void* st)
{
    if (mg_uart_devid == -1) { errno = ENXIO; return -1; }

    sl_create(,mg_io_place_id,,,,,, do_uart_output_byte, 
              sl_glarg(char*,, mg_devinfo.base_addrs[mg_uart_devid]),
              sl_glarg(long*,, mg_devinfo.channels),
              sl_glarg(char,, c)); 
    sl_sync();
    return 0;
}

size_t uart_write(const void* bytes, size_t sz, void* extra)
{
    const char* buf = (const char*)bytes;

    for (size_t i = 0; i < sz; ++i)
        uart_putc(buf[i], extra);
    return sz;
}

static
FILE __stduart = { FK_VIRTUAL, { .virt = { &uart_write, 0 } } };

FILE* stduart = &__stduart;
  
