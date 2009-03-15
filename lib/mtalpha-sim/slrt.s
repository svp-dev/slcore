	.ent _start
	.globl _start
_start:
	.registers 0 0 31 0 0 0
	ldah $l29, 0($l27)	!gpdisp!1
	lda  $l29, 0($l29)	!gpdisp!1

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
	
	.section .rodata
	.ascii "\0sgr_runner:mtalpha-sim:\0"
	
