//
// mtinit.c: this file is part of the SL toolchain.
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

#include <svp/sep.h>
#include <svp/fibre.h>
#include <svp/slr.h>

struct fibre_base_t *__fibre_base;
struct __slr_base_t *__slr_base;

void _lib_init_routine(void* slrbase_init, void* fibrebase_init, void* placeconf)
{
  __slr_base = (struct __slr_base_t*) slrbase_init;
  __fibre_base = (struct fibre_base_t*) fibrebase_init;
  sep_init(placeconf);
}
