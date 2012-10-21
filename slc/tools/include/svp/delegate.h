//
// delegate.h: this file is part of the SL toolchain.
//
// Copyright (C) 2010,2011 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#ifndef SLC_SVP_DELEGATE_H
# define SLC_SVP_DELEGATE_H


/* sl_place_t: data type for place identifiers */

typedef unsigned long sl_place_t;

/* family exit codes */

#define SVP_EXIT_NORMAL 0
#define SVP_EXIT_BREAK  1
#define SVP_EXIT_KILL   2

/* introspection */

extern sl_place_t get_current_place(void);
extern unsigned long get_core_id(void);
extern sl_place_t get_local_place(void);

#if !defined(__AVOID_INLINABLE_PRIMITIVES) && \
    defined(__GNUC__) && !defined(__AVOID_GNUISMS)

#include <svp/compiler.h>
#include <stdlib.h>

alwaysinline unused
sl_place_t __inline_get_current_place(void)
{
#if defined(__slc_os_sim__)
    sl_place_t p;
#if defined(__mips__)
    __asm__("mfc2 %0, $2" : "=r"(p));
#else
    __asm__("getpid %0" : "=r"(p));
#endif
    return p;
#endif
#if defined(__slc_os_host_hlsim__) || defined(__slc_os_host_hlsimd__)
    return hlsim_get_current_place();
#endif 
#if !defined(__slc_multiple_places__)
    return 1;
#endif
   
    // here none of the conditions above apply,
    // but there is no reasonable default. So we 
    // cannot allow the program to continue.
    abort();
}
#define get_current_place() __inline_get_current_place()

alwaysinline unused
unsigned long __inline_get_core_id(void)
{
#if defined(__slc_os_sim__)
    unsigned long p;
#if defined(__mips__)
    __asm__("mfc2 %0, $3" : "=r"(p));
#else
    __asm__("getcid %0" : "=r"(p));
#endif
    return p;
#endif
#if defined(__slc_os_host_hlsim__) || defined(__slc_os_host_hlsimd__)
    return hlsim_get_core_id();
#endif
#if !defined(__slc_multiple_places__)
    return 0;
#endif

    // here none of the conditions above apply,
    // but there is no reasonable default. So we 
    // cannot allow the program to continue.
    abort();
}
#define get_core_id() __inline_get_core_id()

alwaysinline unused
sl_place_t __inline_get_local_place(void)
{
    unsigned long cpid = get_current_place();
    unsigned long cpid_nosize = cpid & (cpid - 1);
    return cpid_nosize | (get_core_id() << 1) | 1;
}
#define get_local_place() __inline_get_local_place()

#endif


#define PLACE_DEFAULT 0
#define PLACE_LOCAL 1

#define sl_default_placement() get_current_place()
#define sl_local_processor_address() get_core_id()
#define sl_placement_size(P) ((P) & -(P))
#define sl_first_processor_address(P) ((P) & ((P) - 1))

#endif
