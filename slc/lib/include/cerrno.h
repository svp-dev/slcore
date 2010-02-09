//
// cerrno.h: this file is part of the SL toolchain.
//
// Copyright (C) 2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//
#ifndef SLC_CERRNO_H
#define SLC_CERRNO_H

#ifdef __mt_freestanding__
extern int errno;

/* used by SL library */
#define ENOMEM          1    /* Cannot allocate memory */
#define EINVAL          2    /* Invalid argument */
#define EFAULT          3    /* Bad address */

/* required by C99 */
#define EDOM            4    /* Numerical argument out of domain */
#define ERANGE          5    /* Result too large */
#define EILSEQ          6    /* Illegal byte sequence */

#else
#ifdef __cplusplus
#include <cerrno>
#else
#include <errno.h>
#endif
#endif

#endif
