!
! callgate.s: this file is part of the SL toolchain.       
!
!
! Copyright (C) 2011 Universiteit van Amsterdam.
!
! This program is free software; you can redistribute it and/or
! modify it under the terms of the GNU General Public License
! as published by the Free Software Foundation; either version 3
! of the License, or (at your option) any later version.
!
! The complete GNU General Public Licence Notice can be found as the
! `COPYING' file in the root directory.
!

!       Implementation for a function call gate. This allows
! 	microthreaded code to invoke plain C functions compiled using
! 	a regular C compiler.
! 
!       The only way to pass data from the parent to a thread with a
! 	full local register window is to use the index register ($l0).
! 
!       Two call gates are provided:
! 
!       __sl_callgate_owntls: the child thread uses its own TLS space.
!        
!       __sl_callgate: the child thread uses the TLS space of its
!       parent. This is appropriate when the caller uses the stack pointer
!       to pass C function arguments (e.g. with varargs)
! 
! Input:  
!       %tl15 - base register for call parameters
!       [%tl15 - 4]  number of register arguments
!       [%tl15 - 8] address of function to call (PV)
!       [%tl15 - 12] first integer argument (value for $a0)
!       ...
!       [%tl15 - 52] last integer argument (value for $a5)
!       ...
! Output:
!       [%tl15 - 8] return int value
! 
        
        .text
        
        .align 64
	.global __sl_callgate_owntls
	.type __sl_callgate_owntls, #function
	.registers 0 0 24 0 0 0
__sl_callgate_owntls:
        ldfp %tl21    ! SP (use local TLS)
        b,a .Ldo_callgate
        nop

        .size __sl_callgate_owntls, .-__sl_callgate_owntls
        
        .align 64
	.global __sl_callgate
	.type __sl_callgate, #function
	.registers 0 0 24 0 0 24
__sl_callgate:
        !MTREG_SET: %tl0
	mov %tl0, %tl21   ! SP (set to be same as parent)

.Ldo_callgate:   
        !MTREG_SET: %tl0,%tl21
        mov %tl0, %tl15       ! base for call parameters
	ld [%tl15 - 4], %tl0  ! load call protocol
	ld [%tl15 - 8], %tl1  ! load PV

        clr %tl16  ! flush callee-save reg
        clr %tl17  ! flush callee-save reg
        clr %tl18  ! flush callee-save reg
        clr %tl19  ! flush callee-save reg
        clr %tl20  ! flush callee-save reg
        clr %tl22  ! flush callee-save reg

        cmp %tl0, 0
	be .Ldocall_a0
        nop
	ld [%tl15 - 12], %tl7

	addcc %tl0, -1, %tl0
	be .Ldocall_a1
        nop
	ld [%tl15 - 20], %tl8

	addcc %tl0, -1, %tl0
	be .Ldocall_a2
        nop
	ld [%tl15 - 28], %tl9

	addcc %tl0, -1, %tl0
	be .Ldocall_a3
        nop
	ld [%tl15 - 36], %tl10

	addcc %tl0, -1, %tl0
	be .Ldocall_a4
        nop
	ld [%tl15 - 44], %tl11

	addcc %tl0, -1, %tl0
	be .Ldocall_a5
        nop
	ld [%tl15 - 52], %tl12

	b,a .Ldocall
        nop

.Ldocall_a0:
	clr %tl7

.Ldocall_a1:
	clr %tl8

.Ldocall_a2:
	clr %tl9

.Ldocall_a3:
	clr %tl10

.Ldocall_a4:
	clr %tl11

.Ldocall_a5:
	clr %tl12

.Ldocall:
	! call function
	call %tl1, 0
         nop

	! save return value(s)
	st %tl7, [%tl15 - 4] 

	end	

        .size __sl_callgate, .-__sl_callgate
	
	