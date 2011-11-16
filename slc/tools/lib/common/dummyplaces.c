//
// dummyplaces.c: this file is part of the SL toolchain.
//
// Copyright (C) 2011 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include <svp/delegate.h>
#include <stdlib.h>

#undef get_current_place
sl_place_t get_current_place(void) {
#if defined(__slc_os_sim__)
    return __inline_get_current_place();
#endif
#if defined(__slc_os_host_seqc__)
    return 1;
#endif
#if defined(__slc_os_host_hlsim__) || defined(__slc_os_host_hlsimd__)
    return hlsim_get_current_place();
#endif


    // here none of the conditions above apply,
    // but there is no reasonable default. So we 
    // cannot allow the program to continue.
    abort();
}

#undef get_core_id
unsigned long get_core_id(void) {
#if defined(__slc_os_sim__)
    return __inline_get_core_id();
#endif
#if defined(__slc_os_host_seqc__)
    return 0;
#endif
#if defined(__slc_os_host_hlsim__) || defined(__slc_os_host_hlsimd__)
    return hlsim_get_core_id();
#endif

    // here none of the conditions above apply,
    // but there is no reasonable default. So we 
    // cannot allow the program to continue.
    abort();
}

#undef get_local_place
sl_place_t get_local_place(void) {
    return __inline_get_local_place();
}


