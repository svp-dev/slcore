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

#ifndef __GNUC__
# warn __GNUC__ is not defined. Some macros here may not work.
#endif

# if defined(__alpha__) || defined(__mtalpha__)
#   define svp_abort() __asm__ __volatile__("nop\n\tnop\n\tpal1d 0" : : : "memory")
# elif defined(__sparc__) || defined(__mtsparc__)
#   define svp_abort() __asm__ __volatile__("nop\n\tnop\n\tunimp" : : : "memory")
# else
#   warn svp_abort() is not defined on this platform.
# endif

#else
# include <stdlib.h>
# define svp_abort() abort()
#endif


#endif // ! SLC_SVP_ABORT_H
