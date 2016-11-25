# filler.s: this file is part of the SL toolchain.
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
	.file "filler.s"
	.section ".startfill"
	.global __filler
	.align 0
__filler:
	.word 0x01000000, 0x30800020, 0x01000000, 0x01000000
	.word 0x01000000, 0x01000000, 0x01000000, 0x01000000
	.word 0x01000000, 0x01000000, 0x01000000, 0x01000000
	.word 0x01000000, 0x01000000, 0x01000000, 0x01000000
	.word 0x01000000, 0x01000000, 0x01000000, 0x01000000
	.word 0x01000000, 0x01000000, 0x01000000, 0x01000000
	.word 0x01000000, 0x01000000, 0x01000000, 0x01000000
	.word 0x01000000, 0x01000000, 0x01000000, 0x01000000
        .type   __filler, #object
        .size   __filler, 128
