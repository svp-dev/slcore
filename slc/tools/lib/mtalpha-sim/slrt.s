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
	#MTREG_SET: $l2,$l16,$l17,$l18,$l27
	ldgp $l29, 0($l27)
	ldfp $l30

	mov $l30, $l15 # set up frame pointer
	clr $l9 # flush callee-save reg
	clr $l10 # flush callee-save reg
	clr $l11 # flush callee-save reg
	clr $l12 # flush callee-save reg
	clr $l13 # flush callee-save reg
	clr $l14 # flush callee-save reg
	fclr $lf2 # flush callee-save reg
	fclr $lf3 # flush callee-save reg
	fclr $lf4 # flush callee-save reg
	fclr $lf5 # flush callee-save reg
	fclr $lf6 # flush callee-save reg
	fclr $lf7 # flush callee-save reg
	fclr $lf8 # flush callee-save reg
	fclr $lf9 # flush callee-save reg
	fclr $lf0 # init FP return reg
	clr $l20 # flush arg reg
	clr $l21 # flush arg reg
	fclr $lf16 # flush arg reg
	fclr $lf17 # flush arg reg
	fclr $lf18 # flush arg reg
	fclr $lf19 # flush arg reg
	fclr $lf20 # flush arg reg
	fclr $lf21 # flush arg reg

	# here $l16, $l17 and $l18 are set by the environment
	# $l2 set by the simulator
	# all 4 are used by the init function
	mov $l2, $l19
	ldq $l27,_lib_init_routine($l29) !literal!1
	jsr $l26,($l27),_lib_init_routine !lituse_jsr!1
	ldgp $l29,0($l26)

	# initialize argc and argv for main()
	lda $l16,1($l31)
	ldq $l17,__pseudo_argv($l29) !literal
	ldq $l18,environ($l29) !literal

	# call main()
	ldq $l27,main($l29) !literal!2
	jsr $l26,($l27),main !lituse_jsr!2
	ldgp $l29,0($l26)
	
	bne $l0, $bad
	nop
	end
$bad:
	ldah $l1, $msg($l29) !gprelhigh
	lda $l2, 115($l31)  # char <- 's'
	lda $l1, $msg($l1) !gprellow
	.align 4
$L0:
	print $l2, 194  # print char -> stderr
	lda $l1, 1($l1)
	ldbu $l2, 0($l1)
	sextb $l2, $l2
	bne $l2, $L0

	print $l0, 130 # print int -> stderr
$fini:	
	nop
	nop
	pal1d 0
	br $fini
	.end _start

	.section .rodata
	.ascii "\0slr_runner:mtalpha-sim:\0"
	.ascii "\0slr_datatag:ppp-mtalpha-sim:\0"

$msg:	
	.ascii "slrt: main returned \0"

        .section .data

        .globl __main_place_id
        .type __main_place_id, @object
        .size __main_place_id, 8
        .align 3
__main_place_id:
        .long 0

        .globl __main_placeinfo
        .type __main_placeinfo, @object
        .size __main_placeinfo, 8
        .align 3
__main_placeinfo:
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
        
