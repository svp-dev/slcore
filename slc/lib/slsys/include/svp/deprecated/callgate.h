//
// callgate.h: this file is part of the SL toolchain.
//
// Copyright (C) 2009,2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#ifndef SLC_SVP_CALLGATE_H
# define SLC_SVP_CALLGATE_H

#ifdef __mt_freestanding__
extern void __sl_callgate_owntls(void);

#if defined(__alpha__) || defined(__mtalpha__)
#define CALL_WITH_INFO(Top, Place)					\
    do {                                                                \
        long __fid, __ret;                                              \
        __asm__ __volatile__("allocate %2, %0\n"                \
                             "\tsetstart %0, %3\n"                      \
                             "\tsetlimit %0, %4\n"                      \
                             "\tcrei %0, 0(%5)\n"                       \
                             "\tsync %0, %1\n"                          \
                             "\tmov %1, $31 #SYNC\n"			\
                             "\trelease %0"                             \
                             : "=r"(__fid), "=r"(__ret)                 \
                             : "r"(Place),                   \
                               "rI"(Top),                               \
                               "rI"(((char*)(void*)(Top))+1),           \
                               "r"(__sl_callgate_owntls)                \
                             : "memory");                               \
    } while(0);

#endif
#endif

#endif // ! SLC_SVP_CALLGATE_H
