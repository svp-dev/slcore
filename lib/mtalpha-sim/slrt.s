# slrt.s: this file is part of the slc project.
#
# Copyright (C) 2008,2009 The SL project.
# All rights reserved.
#
	.ent _start
	.globl _start
_start:
	.registers 0 0 31 0 0 0
	ldah $l29, 0($l27)	!gpdisp!1
	lda  $l29, 0($l29)	!gpdisp!1

	# initialize slr data pointer:
        lda $l1,__slr_base
	stq $l16,0($l1)
	
	# we want to create t_main:
	# main has no shareds, take 1 global = PV
	allocate $l0, 1, 0, 0, 0

	ldq $l1, t_main($l29) !literal
	crei $l0, 0($l1)
	swch
	
	# sync, terminate
	mov $l0, $31
	end	
	.end _start

	.data
	.align 4
	.globl __slr_base
__slr_base:
	.int 0
	.int 0

	.section .rodata
	.ascii "\0slr_runner:mtalpha-sim:\0"
	.ascii "\0slr_datatag:ppp-mtalpha-sim:\0"
	
