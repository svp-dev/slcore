//
// fibre.c: this file is part of the SL toolchain.
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

#include <svp/fibre.h>
#include <svp/iomacros.h>
#include <cstddef.h>

// SLT_RUN:
// SLT_RUN: -f TEST.d1
// SLT_RUN: -f TEST.d2
// SLT_RUN: -f TEST.d3

sl_def(t_main, void)
{
  size_t i, j, nitems;
  int tag;

  for (i = 0; (tag = fibre_tag(i)) != -1; ++i) {
    printf("%d: tag = %d rank = %d\n", i, tag, fibre_rank(i));
    printf("%d: shape = { ", i);
    for (j = 0; j < fibre_rank(i); ++j)
      printf("%d ", fibre_shape(i)[j]);
    printf("}\n");

    nitems = 1;
    for (j = 0; j < fibre_rank(i); ++j)
      nitems *= fibre_shape(i)[j];
    printf("%d: values = { ", i);
    for (j = 0; j < nitems; ++j) {
      switch(tag) {
      case 0: printf("%u ", *((unsigned long*)fibre_data(i)+j)); break;
      case 1: printf("%d ", *((long*)fibre_data(i)+j)); break;
      case 2: printf("%f ", *((double*)fibre_data(i)+j)); break;
      default: printf("? "); break;
      }
    }
    printf("}\n");
  }
  printf("number of Fibre items: %d\n", i);

}
sl_enddef
