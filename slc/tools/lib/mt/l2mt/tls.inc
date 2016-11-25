# tls.s: this file is part of the SL toolchain.
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
	.file "tls.s"
! TLS RESERVATION AREA
        .section ".tls", "", @nobits
        .global __tls_base
        .global __first_tls_top
	.global __tls_end
	.global __stop
        .align 1024
__tls_base:
	.space 1024
__first_tls_top:
	.space 31744 ! space for 32 threads
