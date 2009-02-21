// ptl_debug.h: this file is part of the utcc project.
//
// Copyright (C) 2008 The CSA group.
//
// $Id$
//
#ifndef UTC_PTL_CPP_PTL_DEBUG_H
# define UTC_PTL_CPP_PTL_DEBUG_H

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
#endif // ! UTC_PTL_CPP_PTL_DEBUG_H
