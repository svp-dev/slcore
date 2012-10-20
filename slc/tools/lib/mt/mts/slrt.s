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
        clr     %r16
        clr     %r17
        clr     %r18
        clr     %r19
        clr     %r20
        clr     %r21
        clr     %r22
        clr     %r23
        clr     %o3
        clr     %o4
        clr     %o5

        mov     1, %o0
        sethi   %hi(__pseudo_argv), %o1
        sethi   %hi(environ), %o2
        or      %o1, %lo(__pseudo_argv), %o1
        or      %o2, %lo(environ), %o2
        call    main, 0
         nop

        ! did main terminate with code 0?
        cmp     %o0, 0
        be      .Lsuccess
         nop

        ! otherwise, save the code
        ! and print a message
        mov     %o0, %o1
        sethi   %hi(__exit_code), %o5
        sethi   %hi(__pseudo_stderr), %o3
        st      %o0, [%o5+%lo(__exit_code)]
        or      %o3, %lo(__pseudo_stderr), %o3
        sethi   %hi(slrt_msg), %o5
        mov     115, %o4
        or      %o5, %lo(slrt_msg), %o5
        mov     %o3, %o2
.Lloop:
        ldub    [%o5], %o4
        stb     %o4, [%o3]
        add     %o5, 1, %o5
        ldsb    [%o5], %o4
        cmp     %o4, 0
        bne     .Lloop
         nop

        sra     %o0, 31, %o5
        and     %o5, 15, %o5
        add     %o5, %o0, %o0
        sethi   %hi(slrt_hex), %o5
        sra     %o0, 4, %o0
        or      %o5, %lo(slrt_hex), %o5
        ldub    [%o5+%o0], %o5
        stb     %o5, [%o2]
        stb     %o5, [%o2]
        mov     10, %o5
        stb     %o5, [%o2]

.Lfail:
        ! exit with code in MGSim
        st      %o1, [0x26c]
        ! FIXME: inv ins or something to signal failure if st immediately before does not end execution
        b,a .Lfail
        nop

.Lsuccess:
        nop
        end
        
        .size   _start, .-_start

! MESSAGE STRINGS USED IN _start
        
	.section        ".rodata"
        .align 8
        .type slrt_msg, #object
        .size slrt_msg, 21
slrt_msg:
        .asciz  "slrt: main returned "

        .align 8
        .type   slrt_hex, #object
        .size   slrt_hex, 17
slrt_hex:
        .asciz  "0123456789abcdef"
        
        
! PSEUDO OUTPUT DEVICES
        .section ".bss"
        .common __pseudo_stdout,4,4
        .common __pseudo_stderr,4,4
        .common __exit_code,4,4
        
! PSEUDO ARGV VECTOR
        .global __pseudo_argv
        .section        .rodata.str1.8
        .align 8
__pseudo_progname:
        .asciz  "a.out"
        
        .section        ".data"
        .align 4
        .type   __pseudo_argv, #object
        .size   __pseudo_argv, 8
__pseudo_argv:
        .long   __pseudo_progname
        .long   0

! SLR SPECIAL IDENTIFICATION STRING
        .section        ".rodata"
        .align 8
.Lslrident:      
        .asciz  ""
        .asciz  "slr_runner:mtsparc-sim:"
        .asciz  "slr_datatag:mtsn:"
        
        
! PSEUDO PLACE IDENTIFIERS
        .section ".bss"
        .common __main_place_id,4,4
        
! PSEUDO ENVIRON       
        .section ".bss"
        .common environ,4,4
        

        
        
