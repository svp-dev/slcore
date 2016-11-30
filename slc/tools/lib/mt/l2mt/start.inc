# start.s: this file is part of the SL toolchain.
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
	.file "start.s"
! PROGRAM ENTRY POINT
	.section ".start", "x", @progbits
        .align 128
	.ctlbits 128 0
	.global __stop
	.global _start
        .type _start, #function
        .proc 010
_start:
        ! make SP from end of RAM area
	sethi  %hi(_RAM_END), %o6
	or      %o6, %lo(_RAM_END), %o6
        sub     %o6, 64, %o6
	
	call __copyram, 0
	 nop

	call init_uart, 0
	 nop
	
        ! FIXME: fibre data, env data
	! clr     %o0 ! fibre
        ! clr     %o1 ! initenv
        ! call    sys_init, 0
        ! nop

        sethi   %hi(__argc), %g1
        ld      [%g1+%lo(__argc)], %o0
        sethi   %hi(__argv_ptr), %g1
        ld      [%g1+%lo(__argv_ptr)], %o1
        sethi   %hi(environ), %g1
        ld      [%g1+%lo(environ)], %o2
	mov     %o0, %o0
	mov     %o1, %o1
	mov     %o2, %o2
        call    main, 0
         nop

	! inform the simulator controller.
	sta     %o0, [%r0] 0x84 ! exit code
        ! preserve the exit code in RAM so it can be retrieved later.
        sethi   %hi(__exit_code), %g1
        st      %o0, [%g1+%lo(__exit_code)]
	nop
	nop
	nop

	
