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

[[#]]ifndef SL_SUPPORT_DEFINED
[[#]]define SL_SUPPORT_DEFINED

struct sl_famdata {
  long ix;
  long be;
  long li;
  long st;
  struct sl_famdata *ch;
  int ex;
  void (*f)(struct sl_famdata*);
  void *a;
};

typedef struct sl_famdata *sl_family_t;
typedef int sl_place_t;

[[#]]define PLACE_LOCAL 0
[[#]]define PLACE_DEFAULT 0

[[#]]define SVP_ENOERR 0
[[#]]define SVP_EBROKEN 1
[[#]]define SVP_EKILLED 2

[[#]]endif

#endif // ! SLC_SL_SUPPORT_H
