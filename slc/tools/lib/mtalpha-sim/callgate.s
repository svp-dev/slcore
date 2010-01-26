#
# callgate.s: this file is part of the SL toolchain.       
#
#
# Copyright (C) 2009 The SL project.
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
#       $14 - base register for call parameters
#       [$14 - 8]  number of register arguments
#       [$14 - 16] address of function to call (PV)
#       [$14 - 24] first integer argument (value for $16)
#       [$14 - 32] first float argument (value for $f16)
#       ...
#       [$14 - 104] last integer argument (value for $21)
#       [$14 - 112] last float argument (value for $f21)
# Output:
#       [$14 - 8] return int value
#       [$14 - 16] return float value
# 
        
        .text
        
	.align 4
	.globl __sl_callgate_owntls
	.ent __sl_callgate_owntls
	.registers 0 0 31 0 0 31
__sl_callgate_owntls:
        ldfp $30    # SP (use local TLS)
        br $do_callgate

        .end __sl_callgate_owntls
        
        .align 4
	.globl __sl_callgate
	.ent __sl_callgate
	.registers 0 0 31 0 0 31
__sl_callgate:
        #MTREG_SET: $0
	mov $0, $30   # SP (set to be same as parent)

$do_callgate:   
        #MTREG_SET: $0,$30
        mov $0, $14       # base for call parameters
	ldq $13, -8($14)  # load call protocol
	ldq $27, -16($14) # load PV

	mov $30, $15   # set up frame pointer
        clr $9  # flush callee-save reg
	clr $10 # flush callee-save reg
	clr $11 # flush callee-save reg
	clr $12 # flush callee-save reg
	fclr $f2 # flush callee-save reg
	fclr $f3 # flush callee-save reg
	fclr $f4 # flush callee-save reg
	fclr $f5 # flush callee-save reg
	fclr $f6 # flush callee-save reg
	fclr $f7 # flush callee-save reg
	fclr $f8 # flush callee-save reg
	fclr $f9 # flush callee-save reg
	fclr $f0 # init FP return reg

	beq $13, $docall_16
	ldq $16, -24($14)
	ldt $f16, -32($14)

	subl $13, 1, $13
	beq $13, $docall_17
	ldq $17, -40($14)		
	ldt $f17, -48($14)

	subl $13, 1, $13
	beq $13, $docall_18
	ldq $18, -56($14)
	ldt $f18, -64($14)

	subl $13, 1, $13
	beq $13, $docall_19
	ldq $19, -72($14)
	ldt $f19, -80($14)

	subl $13, 1, $13
	beq $13, $docall_20
	ldq $20, -88($14)
	ldt $f20, -96($14)

	subl $13, 1, $13
	beq $13, $docall_21
	ldq $21, -104($14)
	ldt $f21, -112($14)
	br $docall

$docall_16:
	clr $16
	fclr $f16	
$docall_17:
	clr $17
	fclr $f17
$docall_18:
	clr $18
	fclr $f18
$docall_19:
	clr $19
	fclr $f19
$docall_20:
	clr $20
	fclr $f20
$docall_21:
	clr $21
	fclr $f21
$docall:
	# call function
	jsr $26,($27)

	# save return value(s)
	stq $0, -8($14)
	stt $f0, -16($14)
	end	

	.end __sl_callgate
	
	