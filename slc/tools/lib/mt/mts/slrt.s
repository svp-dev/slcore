# slrt.s: this file is part of the SL toolchain.
#
# Copyright (C) 2011 Universiteit van Amsterdam
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
        .section ".bss"
        .global __tls_base
        .global __first_tls_top
        .lcomm __tls_base,1024
        .lcomm __first_tls_top,261120

! PROGRAM ENTRY POINT
! enter with b,a _start in a thread function
! declared with 24 local registers.
	.section ".text"
        .align 64
        .global _start
        .type _start, #function
        .proc 010
_start:
        ! make SP from TLS reservation
        gettid  %o6
        sethi %hi(__first_tls_top), %g1
        sll     %o6,10,%o6
        add     %o6,%g1,%o6

        sub     %o6, 64, %o6

        ! -L7 sets %r8=%o0, -L6 sets %r7=%g7
        mov     %r7, %o1  ! pass as arg1 to sys_init
        call    sys_init, 0
         nop

        sethi   %hi(__argc), %g1
        ld      [%g1+%lo(__argc)], %o0
        sethi   %hi(__argv_ptr), %g1
        ld      [%g1+%lo(__argv_ptr)], %o1
        sethi   %hi(environ), %g1
        ld      [%g1+%lo(environ)], %o2
        call    main, 0
         nop

        ! inform sys_cleanup of main's result.
        call    sys_cleanup, 0
         nop

        ! exit with code in MGSim
        st      %o0, [0x26c]
        end

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
        .asciz  "\0slr_runner:mtsparc-sim:"
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
