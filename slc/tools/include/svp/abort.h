//
// abort.h: this file is part of the SL toolchain.
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

#ifndef SLC_SVP_ABORT_H
# define SLC_SVP_ABORT_H

#ifdef __mt_freestanding__

#ifdef __slc_os_sim__
#include <svp/mgsim.h>
#define svp_abort() mgsim_control(0, MGSCTL_TYPE_STATACTION, MGSCTL_SA_EXCEPTION, 0)

#elif defined(__slc_arch_leon2mt__) && defined(__slc_os_fpga__)

#include <svp/testoutput.h>

#define svp_abort() do { \
	__dbg_exit_status = -1; \
	__asm__ __volatile__("t_end"); \
	while(1) ;; \
    } while(0)

#else

#error Unsupported arch/os combination

#endif


#else
# include <stdlib.h>
# define svp_abort() abort()
#endif


#endif // ! SLC_SVP_ABORT_H
