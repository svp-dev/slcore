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

#include <svp/mgsim.h>
#define svp_abort() mgsim_control(0, MGSCTL_TYPE_STATACTION, MGSCTL_SA_EXCEPTION, 0)

#else
# include <stdlib.h>
# define svp_abort() abort()
#endif


#endif // ! SLC_SVP_ABORT_H
