# slrt.s: this file is part of the SL toolchain.
#
# Copyright (C) 2016 Universiteit van Amsterdam
#
# This program is free software, you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation, either version 2
# of the License, or (at your option) any later version.
#
# The complete GNU General Public Licence Notice can be found as the
# `COPYING' file in the root directory.
#
        .section        ".rodata"
        .asciz  "\0slr_runner:leon2mt-tbdef:"
        .asciz  "slr_datatag:b32f:\0"
	
	.include "start.inc"
	! The following must immediately follow start.inc.
__stop:
	t_end
	b,a     __stop
	  nop
	.size   _start, .-_start
	
	.include "globals.inc"
	.include "filler.inc"
