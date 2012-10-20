#ifndef SVP_MGSIM_H
#define SVP_MGSIM_H

#define MGSCTL_TYPE_STATACTION  0
#define MGSCTL_SA_CONTINUE      0
#define MGSCTL_SA_EXCEPTION     1
#define MGSCTL_SA_ABORT         2
#define MGSCTL_SA_EXIT          3
#define MGSCTL_CHAN_DEBUG       1

#define MGSCTL_TYPE_MEM    1
#define MGSCTL_MEM_MAP     0
#define MGSCTL_MEM_UNMAP   1


void mgsim_control(unsigned long val, unsigned int type, unsigned int command, unsigned int flags);

#if !defined(__AVOID_INLINABLE_PRIMITIVES)

#ifndef __MGSIM_MMU_IOBASE 
#define __MGSIM_MMU_IOBASE 0x300UL
#endif
#ifndef __MGSIM_ACTION_IOBASE
#define __MGSIM_ACTION_IOBASE 0x260UL
#endif

static inline void
__inline_mgsim_control(unsigned long val, unsigned int type, unsigned int command, unsigned int flags)
{
    volatile unsigned long *base;
    if (type == MGSCTL_TYPE_STATACTION)
        base = (unsigned long*)(__MGSIM_ACTION_IOBASE) + (flags ? 4 : 0) + (command & 3);
    else if (type == MGSCTL_TYPE_MEM)
        base = (unsigned long*)(__MGSIM_MMU_IOBASE) + ((command & 3) << 3) + (flags & 7);
    *base = val;
}

#define mgsim_control(Value, Type, Command, Flags) __inline_mgsim_control(Value, Type, Command, Flags)

#endif

#endif
