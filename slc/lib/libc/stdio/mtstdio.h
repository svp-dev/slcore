#ifndef MT_STDIO_H
#define MT_STDIO_H

// this ought to be entirely replaced
// once proper I/O streams are supported by OS

#include <stdio.h>
#include <string.h>
#include <svp/compiler.h>
#include <svp/testoutput.h>

#if defined(__slc_arch_leon2mt__)  || defined(__slc_arch_leon2__)
extern void uart_putchar(char);
#else
#define uart_putchar(C) ((void)0)
#endif

enum file_kind {
    FK_OUT,
    FK_UART,
    FK_STRING,
    FK_VIRTUAL,
    FK_BUFFER
};

#define STDIO_OUT_BUFFER_SIZE 256

struct __sl_FILE {
    enum file_kind kind;
    union {
        int chan;
        struct {
            char *buf;
            char *end;
            char *cur;
        } str;
        struct {
            size_t (*output)(const void* bytes, size_t sz, void* extra);
            void *extra;
        } virt;
        struct {
            int chan;
            char *ptr;
            size_t *pos;
        } buf;
    } out;
};

#ifndef min
#define min(A, B) (((A) < (B)) ? (A) : (B))
#endif

static inline size_t
__write(FILE *f, const void *bytes, size_t sz)
{
    switch(f->kind)
    {
    case FK_OUT:
        output_bytes(bytes, sz, f->out.chan);
        return sz;
    case FK_UART:
        for (size_t n = 0; n < sz; ++n)
            uart_putchar(((char *)bytes)[n]);
        return sz;
    case FK_STRING:
    {
        size_t n = min(sz, f->out.str.end-f->out.str.cur);
        memcpy(f->out.str.cur, bytes, n);
        f->out.str.cur += n;
        return n;
    }
    case FK_VIRTUAL:
        return f->out.virt.output(bytes, sz, f->out.virt.extra);
    case FK_BUFFER:
    {
        size_t pos = (*f->out.buf.pos) % STDIO_OUT_BUFFER_SIZE;
        for (size_t n = 0; n < sz; ++n)
        {
            f->out.buf.ptr[pos++] = ((char *)bytes)[n];
            f->out.buf.ptr[pos++] = f->out.buf.chan;
            pos = pos % STDIO_OUT_BUFFER_SIZE;
        }
        *f->out.buf.pos = (*f->out.buf.pos) + (sz*2);
        return sz;
    }
    }  
}

static inline size_t
__writes(FILE *f, const char * str)
{
    const char * p = str;
    switch(f->kind)
    {
    case FK_OUT:
    {       
        int chan = f->out.chan;
        while (likely(*p)) output_char(*p++, chan);
    }
    break;
    case FK_UART:
        while (likely(*p)) uart_putchar(*p++);
        break;
    case FK_STRING:
        while (likely(*p && f->out.str.end-f->out.str.cur))
            *f->out.str.cur++ = *p++;
        break;
    case FK_VIRTUAL:
    {
        while (likely(*p)) 
        {
            char c = *p++;
            f->out.virt.output(&c, 1, f->out.virt.extra);
        }
    }      
    break;
    case FK_BUFFER:
    {
        size_t pos = (*f->out.buf.pos) % STDIO_OUT_BUFFER_SIZE;
        while (likely(*p))
        {
            f->out.buf.ptr[pos++] = *p++;
            f->out.buf.ptr[pos++] = f->out.buf.chan;
            pos = pos % STDIO_OUT_BUFFER_SIZE;
        }
        *f->out.buf.pos = (*f->out.buf.pos) + ((size_t)(p - str)*2);
    }
    break;
    }
    return p - str;
}

static inline size_t
__writec(FILE *f, int c)
{
    switch(f->kind)
    {
    case FK_OUT:
        output_char(c, f->out.chan);
        return 1;
    case FK_UART:
        uart_putchar(c);
        return 1;
    case FK_STRING:
        if (f->out.str.end - f->out.str.cur) {
            *f->out.str.cur++ = c;
            return 1;
        }
        else return 0;
    case FK_VIRTUAL:
    {
        char b = c;
        return f->out.virt.output(&b, 1, f->out.virt.extra);
    }
    case FK_BUFFER:
    {
        size_t pos = (*f->out.buf.pos) % STDIO_OUT_BUFFER_SIZE;
        f->out.buf.ptr[pos++] = c;
        f->out.buf.ptr[pos++] = f->out.buf.chan;
        *f->out.buf.pos = (*f->out.buf.pos) + 2;
        return 1;
    }
    }
}



#endif
