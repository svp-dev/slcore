//
// gomt.h: this file is part of the SL toolchain.
//
// Copyright (C) 2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#ifndef SLC_SVP_GOMT_H
# define SLC_SVP_GOMT_H

/* defines INVOKE_MT(PID, FUNC): 

   - equivalent to sl_create(, PID, ,,,,, FUNC); sl_sync()

   - can be used in plain C mode even when create/sync is not
   supported there

   - evaluates to the sync value.
*/

#ifdef __mt_freestanding__

#if defined(__alpha__) || defined(__mtalpha__)
#define INVOKE_MT(Place, Func)                                          \
    ({                                                                  \
        register sl_place_t __pid = (Place);                            \
        register long __ret, __fid;                                     \
        __asm__ __volatile__("allocate %2, %0\n"                        \
                             "\tcrei %0, 0(%3)\n"                       \
                             "\tsync %0, %1\n"                          \
                             "\tmov %1, $31 #MT: sync\n"                 \
                             "\trelease %0"                             \
                             : "=&r"(__fid), "=r"(__ret)                 \
                             : "r"(__pid), "r"(Func));                \
        __ret;                                                          \
    })
#else
#warning INVOKE_MT not defined for this platform.
#endif

#else
 
#define INVOKE_MT(Place, Func)                          \
    ({                                                  \
        sl_place_t __pid = (Place);                     \
        long __ret;                                     \
        sl_create(,__pid,,,,,, (Func)); sl_sync(__ret); \
        __ret;                                          \
    })

#endif

#endif
