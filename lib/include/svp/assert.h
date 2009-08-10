//
// assert.slh: this file is part of the slc project.
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
// $Id$
//
#ifndef __SVP_ASSERT_H__
# define __SVP_ASSERT_H__

#include <svp/iomacros.h>
#include <svp/abort.h>
#include <svp/compiler.h>

#ifdef NDEBUG
#define svp_assert(e) ((void)0)
#else

#define svp_assert(e)  \
    ((void) (likely(e) ? 0 : svp__assert (#e, __FILE__, __LINE__)))
#define svp__assert(e, file, line) \
  ({ \
    printf("%s:%u: failed assertion `%s'\n", file, line, e); \
    svp_abort(); \
   })

#endif

#endif
