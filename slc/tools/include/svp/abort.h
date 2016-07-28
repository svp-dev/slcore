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

#define svp_abort() do {						\
	__asm__ __volatile__("sta %%r0, [%0] 0x80; t_end" : : "r"(4));	\
    } while(1)

#else

#warning svp_abort will not work properly here!
#define svp_abort() do {						\
	__asm__ __volatile__("end");	\
    } while(1)

#endif


#else
# include <stdlib.h>
# define svp_abort() abort()
#endif


#endif // ! SLC_SVP_ABORT_H
