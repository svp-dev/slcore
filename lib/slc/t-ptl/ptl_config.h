// ptl_config.h: this file is part of the utcc project.
//
// Copyright (C) 2008 The CSA group.
//
// $Id$
//
#define UTC_CONFIG_NPTL_PTHREAD_EXIT_BUG

// [ Debug Options ]

// UTC_CONFIG_DEBUG:
// Enable debugging mode, which includes debug prints and human readable family
// identifiers.
// Default: Disabled
// #define UTC_CONFIG_DEBUG

// UTC_CONFIG_DEBUG_MUTEX:
// Enables extra debugging messages and checks on mutexes and conditionals
// Requires: UTC_CONFIG_DEBUG
// Default: Disabled
// #define UTC_CONFIG_DEBUG_MUTEX

// UTC_CONFIG_SIGNALS:
// When enabled, SIGSEGV and SIGABRT are caught and a debug message is printed
// to show which thread caught the signal. Requires UTC_CONFIG_DEBUG to be
// enabled to make sense, not compatible with UTC_CONFIG_XILKERNEL
// Default: Disabled
#define UTC_CONFIG_SIGNALS


// [ Library Tweaks ]

// UTC_CONFIG_BLOCKSIZE_MAX:
// Bounds the default number of actual pthreads allowed to run at the same time
// for a single family, when the Create does not specify a blocksize (which
// overrides this value)
// Default: 128
#define UTC_CONFIG_BLOCKSIZE_MAX 128

// UTC_CONFIG_SET_STACKSIZE:
// Enables setting the default stacksize for a thread
// (not supported on XilKernel)
// Default: Enabled
#define UTC_CONFIG_SET_STACKSIZE

// UTC_CONFIG_DEFAULT_STACKSIZE:
// Default stacksize for a pthread, requires UTC_CONFIG_SET_STACKSIZE to be set.
// This value is lowerbounded by PTHREAD_STACK_MIN, if defined by the pthread
// implementation of the target platform.
// Default: 16384, or PTHREAD_STACK_MIN when not set
#define UTC_CONFIG_DEFAULT_STACKSIZE 16384


// UTC_CONFIG_YIELD:
// Enables yields to the scheduler in the code to optimize performance
// (Recommended when UTC_CONFIG_PTHREAD_CONDITIONAL is configured)
// Default: Enabled
#define UTC_CONFIG_YIELD


// [ Options for limited pthread platforms ]

// UTC_CONFIG_PTHREAD_CONDITIONAL:
// Enables alternative implementation of pthread conditional functions,
// overrides possibly present pthread functions by placing them in the uTC
// namespace. The following are implemented when this option is enabled:
// type pthread_cond_t
// pthread_cond_init
// pthread_cond_destroy
// pthread_cond_wait
// pthread_cond_signal
// pthread_cond_broadcast
//
// Default: Disabled, enabled automatically by UTC_CONFIG_XILKERNEL
// #define UTC_CONFIG_PTHREAD_CONDITIONAL

// UTC_CONFIG_PTHREAD_TSD:
// Enables alternative implementation of pthread thread specific data functions,
// overrides possibly present pthread functions by placing them in the uTC
// namespace. The following are implemented when this option is enabled:
// type pthread_key_t
// pthread_key_create
// pthread_key_delete
// pthread_getspecific
// pthread_setspecific
//
// Default: Disabled, enabled automatically by UTC_CONFIG_XILKERNEL
// #define UTC_CONFIG_PTHREAD_TSD

// UTC_CONFIG_NO_CANCELLATION:
// Enables workarounds to avoid usage of pthread_cancel related functions, use
// when these are not available on the target platform. When enabled, the calls
// to pthread_cancel, pthread_setcancelstate, pthread_testcancel, pthread_setcanceltype,
// pthread_cleanup_push and pthread_cleanup_pop are avoided. This impacts the
// semantics of execution interrupting calls like Break and Kill slightly.
//
// Default: Disabled, enabled automatically by UTC_CONFIG_XILKERNEL
// #define UTC_CONFIG_NO_CANCELLATION
