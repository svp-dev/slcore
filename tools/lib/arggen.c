//
// arggen.sl: this file is part of the slc project.
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

#include <cstddef.h>
#include <svp/testoutput.h>

#ifdef FIBRE_DATA
#include "fdata.c"
#else
#include "data.c"
#endif

sl_def(t_main, void)
{
  output_bytes(&__slr_data, sizeof(__slr_data), 1);
}
sl_enddef
