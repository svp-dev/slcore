//
// callgate.h: this file is part of the SL toolchain.
//
// Copyright (C) 2009 The SL project.
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
        long __fid;                                                     \
        __asm__ __volatile__("allocate %0, 0, 0, 0, 0\n"                \
                             "\tsetstart %0, %2\n"                      \
                             "\tsetlimit %0, %3\n"                      \
                             "\tcrei %0, 0(%4)\n"                       \
                             "\tmov %0, $31 #SYNC\n"			\
                             : "=&r"(__fid)                             \
                             : "0"(Place),                              \
                               "rI"(Top),                               \
                               "rI"(((char*)(void*)(Top))+1),           \
                               "r"(__sl_callgate_owntls)                \
                             : "memory");                               \
    } while(0);

#endif
#endif

#endif // ! SLC_SVP_CALLGATE_H
