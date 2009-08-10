//
// sac1.c: this file is part of the SL toolchain.
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

#include <sac_helpers.h>
#include <svp/iomacros.h>

sl_def(t_main, void)
{
	const char msg[] = "hello world\n";

	char *p1;

	p1 = (char*)malloc(7);
	strncpy(p1, msg, 5);
	p1[5] = '\n';
	p1[6] = '\0';

	puts(p1);

	free(p1);
}
sl_enddef
