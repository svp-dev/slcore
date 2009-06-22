# slrt.s: this file is part of the slc project.
#
# Copyright (C) 2009 The SL project
#
# This program is free software, you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation, either version 2
# of the License, or (at your option) any later version.
#
# The complete GNU General Public Licence Notice can be found as the
# `COPYING' file in the root directory.
#

	.ent _start
	.globl _start
_start:
	.registers 0 0 31 0 0 0
	ldah $l29, 0($l27)	!gpdisp!1
	lda  $l29, 0($l29)	!gpdisp!1

	# initialize slr data pointer:
        ldq $l1,__slr_base($l29) !literal
	stq $l16,0($l1)

	# initialize fibre data pointer:
	ldq $l1,__fibre_base($l29) !literal
	stq $l17,0($l1)
	
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
	.long 0
	.globl __fibre_base
__fibre_base:
	.long 0

	.section .rodata
	.ascii "\0slr_runner:mtalpha-sim:\0"
	.ascii "\0slr_datatag:ppp-mtalpha-sim:\0"
	
