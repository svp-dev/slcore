	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 1
	.abicalls
	.text
	.align	2
	.globl	__start
	.set	nomips16
	.ent	__start
	.type	__start, @function
__start:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 16, gp= 8
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
        mfc2    $sp, $4
	addiu	$sp,$sp,-32
        move    $fp,$sp
	sw	$31,28($sp)
	.cprestore	16
        # slr provides args in $8(fdata), $7(initenv)
        # but sysinit expects args in a0=$4(fdata), a1=$5(initenv)
        move    $4,$8 # a0($4): slr fdata file
        move    $5,$7 # a1($5): slr initenv
	jal	sys_init
	la	$2,__argc
	lw	$4,0($2)
	la	$2,__argv_ptr
	lw	$5,0($2)
	la	$2,environ
	lw	$6,0($2)
	jal	main

        move    $4,$2 # provide main's exit code (v0) as sys_cleanup's a0
        jal     sys_cleanup

        # exit with code
        sw      $2,0x26c($0)
	.end	__start
	.size	__start, .-__start

	.section .rodata
	.ascii "\0slr_runner:mipsel-sim:\0"
	.ascii "\0slr_datatag:l32d:\0"

        .globl	__main_place_id
	.section	.bss,"aw",@nobits
	.align	2
	.type	__main_place_id, @object
	.size	__main_place_id, 4
__main_place_id:
	.space	4
	.globl	__argc
	.data
	.align	2
	.type	__argc, @object
	.size	__argc, 4
__argc:
	.word	1
	.globl	__argv_ptr
	.section	.data.rel.local,"aw",@progbits
	.align	2
	.type	__argv_ptr, @object
	.size	__argv_ptr, 4
__argv_ptr:
	.word	__pseudo_argv

	.comm	environ,4,4
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC1:
	.ascii	"a.out\000"
	.section	.data.rel.local
	.align	2
	.type	__pseudo_argv, @object
	.size	__pseudo_argv, 8
__pseudo_argv:
	.word	$LC1
	.word	0
	.ident	"GCC: (GNU) 4.7.1"
