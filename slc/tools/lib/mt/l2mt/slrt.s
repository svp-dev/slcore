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

! TLS RESERVATION AREA
        .section ".tls"
        .global __tls_base
        .global __first_tls_top
        .align 1024
        .lcomm __tls_base,1024
        .lcomm __first_tls_top,261120

! PROGRAM ENTRY POINT
	.section ".init"
        .align 128
	.ctlbits 128 0
	.global _start
        .type _start, #function
        .proc 010
_start:
        ! make SP from TLS reservation
        t_get_tid  %o6
        sethi   %hi(__first_tls_top), %g1
        sll     %o6,10,%o6
        add     %o6,%g1,%o6

        sub     %o6, 64, %o6

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
        call    main, 0
         nop

        ! inform sys_cleanup of main's result.
        ! call    sys_cleanup, 0
        ! nop

	sethi   %hi(__dbg_exit_status), %r1
	st      %o0, [%r1+%lo(__dbg_exit_status)]
	t_end
.L2:
	b,a     .L2

        .size   _start, .-_start

! PSEUDO OUTPUT DEVICES
        .section ".bss"
        .common __pseudo_stdout,4,4
        .common __pseudo_stderr,4,4
        .common __exit_code,4,4

! ARGC, ARGV, SLR CONTROL, ENVIRON
        .section        ".rodata"
        .align 8
.LLC0:
        .asciz  "\0slr_runner:leon2mt-fpga:"
        .asciz  "slr_datatag:b32f:"
        .align 8
.LLC1:
        .asciz  "a.out"
	
        .section        ".data"
        .align 4
        .type   __pseudo_argv, #object
        .size   __pseudo_argv, 8
__pseudo_argv:
        .long   .LLC1
        .long   0
        .global __argv_ptr
        .align 4
        .type   __argv_ptr, #object
        .size   __argv_ptr, 4
__argv_ptr:
        .long   __pseudo_argv
        .global __argc
        .align 4
        .type   __argc, #object
        .size   __argc, 4
__argc:
        .long   1
        .common __main_place_id,4,4
        .common environ,4,4
