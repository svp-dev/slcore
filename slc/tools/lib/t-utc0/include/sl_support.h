//
// sl_support.h: this file is part of the SL toolchain.
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

#ifndef SLC_SL_SUPPORT_H
# define SLC_SL_SUPPORT_H

[[#]]define __svp_break(Val) do { \
    __asm__ __volatile__("break %0" : : "rI"(Val)); \
    while(1) __asm__ __volatile__("nop\n\tswch"); \
  } while(0);

[[#]]define __svp_breakf(Val) do { \
    __asm__ __volatile__("breakf %0" : : "f"(Val)); \
    while(1) __asm__ __volatile__("nop\n\tswch"); \
  } while(0);

[[#]]define __svp_kill(Val) __asm__ __volatile__("kill %0" : : "r"(Val))

typedef long sl_family_t;
typedef long sl_place_t;

[[#]]define PLACE_LOCAL   0
[[#]]define PLACE_DEFAULT 1

[[#]]define SVP_ENORMAL 0
[[#]]define SVP_EBROKEN 1
[[#]]define SVP_EKILLED 2

#endif // ! SLC_SL_SUPPORT_H
