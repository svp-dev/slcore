# slrt.s: this file is part of the SL toolchain.
#
# Copyright (C) 2009,2010 The SL project
#
# This program is free software, you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation, either version 2
# of the License, or (at your option) any later version.
#
# The complete GNU General Public Licence Notice can be found as the
# `COPYING' file in the root directory.
#

	.text
	.ent _start
	.globl _start
	.registers 0 0 31 0 0 31
_start:
	#MTREG_SET: $l2,$l5,$l6,$l7,$l27
	ldgp $l17, 0($l27)
	ldfp $l18

	mov $l18, $l16 # set up frame pointer
	clr $l8 # flush callee-save reg
	clr $l9 # flush callee-save reg
	clr $l10 # flush callee-save reg
	clr $l11 # flush callee-save reg
	clr $l12 # flush callee-save reg
	clr $l13 # flush callee-save reg
	fclr $lf11 # flush callee-save reg
	fclr $lf12 # flush callee-save reg
	fclr $lf13 # flush callee-save reg
	fclr $lf14 # flush callee-save reg
	fclr $lf15 # flush callee-save reg
	fclr $lf16 # flush callee-save reg
	fclr $lf17 # flush callee-save reg
	fclr $lf18 # flush callee-save reg
	fclr $lf3 # init FP return reg
        clr $l2 # flush arg reg
	clr $l3 # flush arg reg
        clr $l4 # flush arg reg
	fclr $lf5 # flush arg reg
	fclr $lf6 # flush arg reg
	fclr $lf7 # flush arg reg
	fclr $lf8 # flush arg reg
	fclr $lf9 # flush arg reg
	fclr $lf10 # flush arg reg

	# here $l7(a0), $l6(a1), $l5(a2) are set by the environment
	# all 3 are used by the init function
	ldq $l14,sys_init($l17) !literal!1
	jsr $l15,($l14),sys_init !lituse_jsr!1
	ldgp $l17,0($l15)

	# initialize argc and argv for main()
	lda $l7,1($l31)
	ldq $l6,__pseudo_argv($l17) !literal
	ldq $l5,environ($l17) !literal

	# call main()
	ldq $l14,main($l17) !literal!2
	jsr $l15,($l14),main !lituse_jsr!2
	ldgp $l17,0($l15)

	bne $l1, $bad
        nop
	end
$bad:
	ldah $l3, $msg($l17) !gprelhigh
	lda $l2, 115($l31)  # char <- 's'
	lda $l3, $msg($l3) !gprellow
	.align 4
$L0:
	print $l2, 194  # print char -> stderr
	lda $l3, 1($l3)
	ldbu $l2, 0($l3)
	sextb $l2, $l2
	bne $l2, $L0

	print $l1, 130 # print int -> stderr
        lda $l1, 10($l31) # NL
	print $l1, 194  # print char -> stderr
$fini:	
	nop
	nop
	pal1d 0
	br $fini
	.end _start

	.section .rodata
	.ascii "\0slr_runner:mtalpha-sim:\0"
	.ascii "\0slr_datatag:mta:\0"

$msg:	
	.ascii "slrt: main returned \0"

        .section .data

        .globl __main_place_id
        .type __main_place_id, @object
        .size __main_place_id, 8
        .align 3
__main_place_id:
        .long 0

        .globl environ
        .type environ, @object
        .size environ, 8
        .align 3
environ:
        .long 0
        
	.globl __pseudo_argv
        .type __pseudo_argv, @object
        .size __pseudo_argv, 16
	.align 3
__pseudo_argv:
	.long $progname
	.long 0
        
$progname:
	.ascii "a.out\0"
        
