//
// benchmark.h: this file is part of the SL toolchain.
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

#ifndef SLC_LIB_BENCHMARK_H
# define SLC_LIB_BENCHMARK_H

#include <svp/sep.h>

struct benchmark_state {
  void* data;
#if SVP_HAS_SEP
  struct placeinfo *place;
#endif
};

struct benchmark {
  const char *title;
  const char *author;
  const char *description;
  sl_decl((*initialize), void, sl_glparm(struct benchmark_state*, state));
  sl_decl((*prepare), void, sl_glparm(struct benchmark_state*, state));
  sl_decl((*work), void, sl_glparm(struct benchmark_state*, state));
  sl_decl((*output), void, sl_glparm(struct benchmark_state*, state));
  sl_decl((*teardown), void, sl_glparm(struct benchmark_state*, state));
};

sl_decl(run_benchmark, void, sl_glparm(struct benchmark*, b));

#endif // ! SLC_LIB_BENCHMARK_H
