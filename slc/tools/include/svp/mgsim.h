#ifndef SVP_MGSIM_H
#define SVP_MGSIM_H


#ifndef __mt_freestanding__
#warning nothing to see here.
#endif


#define mgsim_outfloat(F, Stream, Precision) __asm__ __volatile("printf %0, %1" : : "f"(F), \
                                                                "r"((Stream) | (Precision << 4)))

#define mgsim_control(Value, Type, Command, Flags)      \
    __asm__ __volatile__("print %0, %1" : : "r"(Value), "rI"(((Command)<<6) | (Type) | (Flags)))

#define MGSCTL_TYPE_GFX     0x20
#define MGSCTL_GFX_PUTPIXEL 0
#define MGSCTL_GFX_RESIZE   1
#define MGSCTL_GFX_SSHOT    2

#define MGSCTL_GFX_PUTPIXEL_XY     0x00
#define MGSCTL_GFX_PUTPIXEL_OFFSET 0x10

#define MGSCTL_GFX_SSHOT_TIMESTAMP 0x10
#define MGSCTL_GFX_SSHOT_TINFO     0x08

#define MGSCTL_TYPE_STATACTION  0x10
#define MGSCTL_SA_CONTINUE      0
#define MGSCTL_SA_EXCEPTION     1
#define MGSCTL_SA_ABORT         2
#define MGSCTL_SA_EXIT          3

#define MGSCTL_TYPE_MEM    0x08
#define MGSCTL_MEM_MAP     0
#define MGSCTL_MEM_UNMAP   1

#define MGSCTL_TYPE_PRINT  0x00
#define MGSCTL_PRINT_UDEC  0
#define MGSCTL_PRINT_HEX   1
#define MGSCTL_PRINT_SDEC  2
#define MGSCTL_PRINT_BYTE  3

#define MGSCTL_CHAN_DEBUG  0
#define MGSCTL_CHAN_STDOUT 1
#define MGSCTL_CHAN_STDERR 2

#endif
