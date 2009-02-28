	.ent _start
	.globl _start
_start:
	.registers 0 0 31 0 0 0
	ldah $29, 0($27)	!gpdisp!1
	lda  $29, 0($29)	!gpdisp!1

	# we want to create t_main
	# $1-$3 = shareds
	# $4-$9 = globals
	# $0 = sync
	allocate $0, 4, 1, 0, 0
	
	ldq $9, t_main($29) !literal
	crei $0, 0($9)
	swch
	mov $31, $1
	mov $31, $2
	mov $31, $3
	
	# sync, terminate
	mov $0, $31
	end	
	.end _start
	
	.section .rodata
	.ascii "\0sgr_runner:mtalpha-sim:\0"
	
