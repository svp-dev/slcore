//
// argslot.h: this file is part of the SL toolchain.
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

#ifndef SLC_SVP_ARGSLOT_H
# define SLC_SVP_ARGSLOT_H

#include <cstdint.h>

/* argument slot suitable to simulate
   C's variable argument lists */

typedef union {
  uint8_t ub;
  int8_t sb;
  uint16_t us;
  int16_t ss;
  uint32_t ul;
  int32_t sl;
  uint64_t uq;
  int64_t sq;
  float f;
  double d;
  void *ptr;
} svp_arg_slot;

#endif // ! SLC_SVP_ARGSLOT_H
