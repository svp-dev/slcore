#
# callgate.s: this file is part of the SL toolchain.       
#
#
# Copyright (C) 2009,2010 The SL project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 3
# of the License, or (at your option) any later version.
#
# The complete GNU General Public Licence Notice can be found as the
# `COPYING' file in the root directory.
#

#       Implementation for a function call gate. This allows
# 	microthreaded code to invoke plain C functions compiled using
# 	a regular C compiler.
# 
#       The only way to pass data from the parent to a thread with a
# 	full local register window is to use the index register ($l0).
# 
#       Two call gates are provided:
# 
#       __sl_callgate_owntls: the child thread uses its own TLS space.
#        
#       __sl_callgate: the child thread uses the TLS space of its
#       parent. This is appropriate when the caller uses the stack pointer
#       to pass C function arguments (e.g. with varargs)
# 
# Input:  
#       $16 - base register for call parameters
#       [$16 - 8]  number of register arguments
#       [$16 - 16] address of function to call (PV)
#       [$16 - 24] first integer argument (value for $a0)
#       [$16 - 32] first float argument (value for $fa0)
#       ...
#       [$16 - 104] last integer argument (value for $a5)
#       [$16 - 112] last float argument (value for $fa5)
# Output:
#       [$16 - 8] return int value
#       [$16 - 16] return float value
# 
        
        .text
        
	.align 4
	.globl __sl_callgate_owntls
	.ent __sl_callgate_owntls
	.registers 0 0 19 0 0 19
__sl_callgate_owntls:
        ldfp $18    # SP (use local TLS)
        br $do_callgate

        .end __sl_callgate_owntls
        
        .align 4
	.globl __sl_callgate
	.ent __sl_callgate
	.registers 0 0 19 0 0 19
__sl_callgate:
        #MTREG_SET: $0
        .set noat
	mov $0, $18   # SP (set to be same as parent)

$do_callgate:   
        #MTREG_SET: $0,$18
        .set noat
        mov $0, $16       # base for call parameters
	ldq $13, -8($16)  # load call protocol
	ldq $14, -16($16) # load PV

        clr $1  # init int return reg (RV)
        clr $8  # flush callee-save reg
        clr $9  # flush callee-save reg
	clr $10 # flush callee-save reg
	clr $11 # flush callee-save reg
	clr $12 # flush callee-save reg
	fclr $f3 # init FP return reg
	fclr $f4 # init FP return reg
	fclr $f11 # flush callee-save reg
	fclr $f12 # flush callee-save reg
	fclr $f13 # flush callee-save reg
	fclr $f14 # flush callee-save reg
	fclr $f15 # flush callee-save reg
	fclr $f16 # flush callee-save reg
	fclr $f17 # flush callee-save reg
	fclr $f18 # flush callee-save reg

	beq $13, $docall_a0
	ldq $7, -24($16)
	ldt $f10, -32($16)

	subl $13, 1, $13
	beq $13, $docall_a1
	ldq $6, -40($16)		
	ldt $f9, -48($16)

	subl $13, 1, $13
	beq $13, $docall_a2
	ldq $5, -56($16)
	ldt $f8, -64($16)

	subl $13, 1, $13
	beq $13, $docall_a3
	ldq $4, -72($16)
	ldt $f7, -80($16)

	subl $13, 1, $13
	beq $13, $docall_a4
	ldq $3, -88($16)
	ldt $f6, -96($16)

	subl $13, 1, $13
	beq $13, $docall_a5
	ldq $2, -104($16)
	ldt $f5, -112($16)
	br $docall

$docall_a0:
	clr $7
	fclr $f10	
$docall_a1:
	clr $6
	fclr $f9
$docall_a2:
	clr $5
	fclr $f8
$docall_a3:
	clr $4
	fclr $f7
$docall_a4:
	clr $3
	fclr $f6
$docall_a5:
	clr $2
	fclr $f5
$docall:
	# call function
	jsr $15,($14)

	# save return value(s)
	stq $1, -8($16)
	stt $f3, -16($16)
	end	

	.end __sl_callgate
	
	