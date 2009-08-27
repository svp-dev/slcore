//
// ptl_debug.h: this file is part of the SL toolchain.
//
// Copyright (C) 2008, 2009 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#ifndef SLC_PTL_DEBUG_H
# define SLC_PTL_DEBUG_H

#include "ptl_deps.h"

#ifdef UTC_CONFIG_DEBUG


namespace uTC
{
    // Library compiled in debug mode, define init and cleanup function for debug
    void debug_init();
    void debug_cleanup();

    // Debug print mutex to guard output
    extern pthread_mutex_t g_debug_mutex;

    // Debug print macro, uses global g_debug_mutex to guarantee only one debug
    // print at the time, and is not cancellable.

    #ifdef UTC_CONFIG_DEBUG_MUTEX
        // Debug versions of mutex operations, for extra debugging
        int __LOCK    (const char* file, int line, const char* name, pthread_mutex_t* mutex);
        int __TRYLOCK (const char* file, int line, const char* name, pthread_mutex_t* mutex);
        int __UNLOCK  (const char* file, int line, const char* name, pthread_mutex_t* mutex);
        int __CONDWAIT(const char* file, int line, const char* name, pthread_cond_t* cond, pthread_mutex_t* mutex);
        int __DESTROY (const char* file, int line, const char* name, pthread_mutex_t* mutex);
    #endif

// End namespace uTC
}

// End #ifdef UTC_CONFIG_DEBUG
#endif

// End #ifndef UTC_PTL_DEBUG_H
#endif // ! SLC_PTL_DEBUG_H
