// ptl_deps.h: this file is part of the utcc project.
//
// Copyright (C) 2008 The CSA group.
//
// $Id$
//
#ifndef UTC_PTL_CPP_PTL_DEPS_H
# define UTC_PTL_CPP_PTL_DEPS_H

// This is the only file that will include config.h
#include "ptl_config.h"

// Check for conflicting platform options with XilKernel
#ifdef UTC_CONFIG_XILKERNEL
  #ifdef UTC_CONFIG_SOLARIS_8
    #error Conflicting configuration: UTC_CONFIG_XILKERNEL and UTC_CONFIG_SOLARIS_8
  #endif
  #ifdef UTC_CONFIG_SIGNALS
    #error Conflicting configuration: UTC_CONFIG_XILKERNEL and UTC_CONFIG_SIGNALS, XilKernel has no signal support
  #endif
  #ifdef UTC_CONFIG_SET_STACKSIZE
    #error Conflicting configuration: UTC_CONFIG_XILKERNEL and UTC_CONFIG_SET_STACKSIZE, XilKernel has no support for setting stack size
  #endif

  // This workaround doesnt make sense under Xilinx, so if enabled by accident
  // let's disable it. It would only impact performance or binary size otherwise
  // anyway (not functionality)
  #ifdef UTC_CONFIG_NPTL_PTHREAD_EXIT_BUG
    #undef UTC_CONFIG_NPTL_PTHREAD_EXIT_BUG
  #endif

  // Enable the Xilinx specific settings, if not already enabled
  #ifndef UTC_CONFIG_PTHREAD_CONDITIONAL
    #define UTC_CONFIG_PTHREAD_CONDITIONAL
  #endif
  #ifndef UTC_CONFIG_PTHREAD_TSD
    #define UTC_CONFIG_PTHREAD_TSD
  #endif
  #ifndef UTC_CONFIG_NO_CANCELLATION
    #define UTC_CONFIG_NO_CANCELLATION
  #endif

  // Special XilKernel includes are done in System includes section below
#endif


// Make sure Debug is turned on when Debugging on Mutexes is requested
#if defined UTC_CONFIG_DEBUG_MUTEX && !defined UTC_CONFIG_DEBUG
  #define UTC_CONFIG_DEBUG
#endif

// Be compatible with DEBUG/NDEBUG standard/convention
#if defined DEBUG && !defined UTC_CONFIG_DEBUG
  #define UTC_CONFIG_DEBUG
#endif

#if !defined UTC_CONFIG_DEBUG && !defined NDEBUG
  #define NDEBUG
#endif


// If Debug is enabled, set the according debug macros
#ifdef UTC_CONFIG_DEBUG
  // Debug mode, set the DPRINT macro
  // However, with cancelation disabled we need a different version
  #ifdef UTC_CONFIG_NO_CANCELLATION

    // Safe Debug print with debug mutex; without cancellation
    #define DPRINT(s) \
    { \
        pthread_mutex_lock(&g_debug_mutex); \
        std::cerr << "[" << pthread_self() << "] " << s << std::endl; \
        pthread_mutex_unlock(&g_debug_mutex); \
    }

  #else
    // Safe Debug print with debug mutex; with cancellation workaround,
    // as we don't want to be able to get cancelled in a debug print
    #define DPRINT(s) \
    { \
        int state; \
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &state); \
        pthread_mutex_lock(&g_debug_mutex); \
        std::cerr << "[" << pthread_self() << "] " << s << std::endl; \
        pthread_mutex_unlock(&g_debug_mutex); \
        pthread_setcancelstate(state, NULL); \
    }

  #endif

  // Debug mode only execution macro
  #define DEXEC(s) s

#else
  // Normal mode, define empty DPRINT macro
  #define DPRINT(s)
  #define DEXEC(s)
#endif


// If debugging mutexes is enabled, set the according macro's to
// the debugging wrapper functions
#ifdef UTC_CONFIG_DEBUG_MUTEX
  // Debug versions of mutex operations
  #define LOCK(mutex)           (__LOCK    (__FILE__, __LINE__, (#mutex), (mutex)))
  #define TRYLOCK(mutex)        (__TRYLOCK (__FILE__, __LINE__, (#mutex), (mutex)))
  #define UNLOCK(mutex)         (__UNLOCK  (__FILE__, __LINE__, (#mutex), (mutex)))
  #define CONDWAIT(cond, mutex) (__CONDWAIT(__FILE__, __LINE__, (#mutex), (cond), (mutex)))
  #define DESTROY(mutex)        (__DESTROY (__FILE__, __LINE__, (#mutex), (mutex)))
#else
  // Non-debug versions of mutex operations
  #define LOCK(mutex)           pthread_mutex_lock(mutex)
  #define TRYLOCK(mutex)        pthread_mutex_trylock(mutex)
  #define UNLOCK(mutex)         pthread_mutex_unlock(mutex)
  #define CONDWAIT(cond, mutex) pthread_cond_wait((cond), (mutex))
  #define DESTROY(mutex)        pthread_mutex_destroy(mutex)

// End #ifdef UTC_CONFIG_DEBUG_MUTEX
#endif


// Should we set the UTC_YIELD macro?
#ifdef UTC_CONFIG_YIELD

  // Yes, but the Xilinx or the Posix version?
  #ifdef UTC_CONFIG_XILKERNEL
    // Xilkernel version
    #define UTC_YIELD(); yield();

  #else
    // General Posix version
    #define UTC_YIELD(); sched_yield();

  #endif

#else
  // Nope, define an empty one
  #define UTC_YIELD();

// End #ifdef UTC_CONFIG_YIELD
#endif

// Set macros for cancellation support
#ifdef UTC_CONFIG_NO_CANCELLATION
 #define EnableCancellation()
 #define DisableCancellation()
#else
 #define EnableCancellation()       __EnableCancellation(__FILE__, __LINE__)
 #define DisableCancellation()      __DisableCancellation(__FILE__, __LINE__)

// End #ifdef UTC_CONFIG_NO_CANCELLATION
#endif


// Check the stacksize macro if stacksize setting enabled, and set to default
#if defined UTC_CONFIG_SET_STACKSIZE && !defined UTC_CONFIG_DEFAULT_STACKSIZE
  #define UTC_CONFIG_DEFAULT_STACKSIZE PTHREAD_STACK_MIN
#endif


/*****************************************************************************\
*                                                                             *
* System Includes Section                                                     *
* All system include headers should go here, after we figured out above which *
* exact configuration options we will be using.                               *
*                                                                             *
\*****************************************************************************/


// Xilkernel requires xmk.h as the first system include
#ifdef UTC_CONFIG_XILKERNEL
  #include "xmk.h"
  #include <os_config.h>
  #include <sys/process.h>

// End of #ifdef UTC_CONFIG_XILKERNEL
#endif

// Of course very important, include the System's Pthread library headers ;)
#include <pthread.h>

// INT_MAX might be used in a create definition created by the translator
#include <climits>

// Signal only needs to be included when signal handlers are installed or
// the Solaris 8 ALARM signal bug workaround is enabled
#if defined UTC_CONFIG_SOLARIS_8 || defined UTC_CONFIG_SIGNALS
    #include <csignal>
#endif

// We need to include sched.h if we plan to use sched_yield
#if defined UTC_CONFIG_YIELD && !defined UTC_CONFIG_XILKERNEL
  #include <sched.h>
#endif

// Conditional implementation tries to respect the Posix errno return values
#include <cerrno>

// STL stuff

// m_threads in FamilyBase and TSD is implemented with a STL map
#include <map>


// Stream operators are only used within debug prints or to create friendly
// readable Family names in m_id in FamilyBase, which only are enabled with
// debug mode as they are only used for printing in debug messages.
#ifdef UTC_CONFIG_DEBUG

  // Stream operators
  #include <sstream>
  #include <iostream>

  // Used for Error printing (strerror)
  //#include <string.h>

// End of #ifdef UTC_CONFIG_DEBUG
#endif


// End of #ifndef UTC_DEPENDENCIES_H
#endif // ! UTC_PTL_CPP_PTL_DEPS_H
