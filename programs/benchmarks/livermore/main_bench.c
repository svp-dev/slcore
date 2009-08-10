//
// main_bench.c: this file is part of the SL toolchain.
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

#include <svp/assert.h>
#include <svp/fibre.h>

sl_def(t_main, void)
{
	// first Fibre item is iteration count
	svp_assert(fibre_tag(0) == 0 && fibre_rank(0) == 0);
	unsigned loops = *(unsigned long*)fibre_data(0);

	// 2nd Fibre item is problem size
	svp_assert(fibre_tag(1) == 0 && fibre_rank(1) == 0);
	unsigned psize = *(unsigned long*)fibre_data(1);

	//set the required problem size
	//if the value is 0, then use default PSIZE
	//also make sure that the PSIZE is less that the
	//maximum size of the data arrays

	if (psize > 0) {
	  svp_assert(psize < LARGEST);
	  inner[KERNEL] = psize;
	}

	while (loops--)
	{
	  sl_create(,,,,,,,ACTUALKERNEL);
	  sl_sync();
	}
}
sl_enddef
