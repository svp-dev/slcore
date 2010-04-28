//
// sac1.c: this file is part of the SL toolchain.
//
// Copyright (C) 2009,2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include <sac_helpers.h>
#include <cstdio.h>
#include <svp/compiler.h>

noinline char *pass(char *reg) { return reg; }

sl_def(t_main, void)
{
	const char msg[] = "hello world\n";

	char *p1, *p2;

	p1 = (char*)malloc(7);
	p2 = (char*)alloca(2);

	strncpy(p1, msg, 5);
	p1[5] = '\0';

	p2[0] = '\n';
	p2[1] = '\0';


	puts(p1); 
	puts(p2);

	free(p1);

	p2 = pass(p2);
#ifdef safe_free
	safe_free(p2);
#endif
}
sl_enddef
