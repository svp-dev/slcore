#ifndef SVP_MGSIM_H
#define SVP_MGSIM_H

void mgsim_control(unsigned long val, unsigned int type, unsigned int command, unsigned int flags);
#if !defined(__AVOID_INLINABLE_PRIMITIVES)
#define __inline_mgsim_control(Value, Type, Command, Flags)      \
    ((void) ({ __asm__ __volatile__("print %0, %1" : : "r"(Value), "rI"(((Command)<<6) | (Type) | (Flags))); }) )
#define mgsim_control(Value, Type, Command, Flags) __inline_mgsim_control(Value, Type, Command, Flags)
#endif

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

void mgsim_outfloat(double val, unsigned int stream, unsigned int prec);
#if !defined(__AVOID_INLINABLE_PRIMITIVES)
#if !defined(__slc_soft_float__)
#define __inline_mgsim_outfloat(F, Stream, Precision) \
    ((void) ({ __asm__ __volatile("printf %0, %1" : : "f"(F), "r"((Stream) | (Precision << 4))); }) )
#else
#define __inline_mgsim_outfloat(F, Stream, Precision) \
    mgsim_control(F, MGSCTL_TYPE_PRINT, MGSCTL_PRINT_HEX, Stream)
#endif
#define mgsim_outfloat(F, Stream, Precision) __inline_mgsim_outfloat(F, Stream, Precision)
#endif


#endif
