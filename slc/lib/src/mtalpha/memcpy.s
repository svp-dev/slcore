	.set noreorder
	.set volatile
	.set noat
	.set nomacro
	.arch ev4
	.text
	.align 2
	.ent __memcpy_unaligned_up
$__memcpy_unaligned_up..ng:
__memcpy_unaligned_up:
	.frame $15,80,$26,0
	.mask 0x4008000,-80
	lda $30,-80($30)
	stq $26,0($30)
	stq $15,8($30)
	mov $30,$15
	.prologue 0
	stq $16,48($15)
	stq $17,56($15)
	stq $18,64($15)
	br $31,$L2
$L5:
	ldq $1,64($15)
	ble $1,$L10
$L3:
	ldq $1,64($15)
	lda $1,-1($1)
	stq $1,64($15)
	ldq $1,48($15)
	ldq $2,56($15)
	lda $3,1($2)
	ldq_u $2,0($2)
	extqh $2,$3,$2
	sra $2,56,$2
	bis $31,$2,$4
	ldq_u $3,0($1)
	mov $1,$2
	mskbl $3,$2,$3
	insbl $4,$2,$2
	bis $2,$3,$2
	stq_u $2,0($1)
	ldq $1,48($15)
	lda $1,1($1)
	stq $1,48($15)
	ldq $1,56($15)
	lda $1,1($1)
	stq $1,56($15)
$L2:
	ldq $1,48($15)
	and $1,7,$1
	bne $1,$L5
	ldq $1,64($15)
	lda $1,-8($1)
	stq $1,64($15)
	ldq $1,64($15)
	blt $1,$L6
	ldq $1,56($15)
	.set	macro
 # 77 "memcpy.c" 1
	ldq_u $1,0($1)
 # 0 "" 2
	.set	nomacro
	stq $1,32($15)
$L7:
	ldq $1,56($15)
	lda $1,8($1)
	.set	macro
 # 80 "memcpy.c" 1
	ldq_u $1,0($1)
 # 0 "" 2
	.set	nomacro
	stq $1,24($15)
	ldq $1,64($15)
	lda $1,-8($1)
	stq $1,64($15)
	ldq $1,32($15)
	ldq $2,56($15)
	.set	macro
 # 82 "memcpy.c" 1
	extql $1,$2,$1
 # 0 "" 2
	.set	nomacro
	stq $1,32($15)
	ldq $1,24($15)
	ldq $2,56($15)
	.set	macro
 # 85 "memcpy.c" 1
	extqh $1,$2,$1
 # 0 "" 2
	.set	nomacro
	stq $1,16($15)
	ldq $1,56($15)
	lda $1,8($1)
	stq $1,56($15)
	ldq $1,48($15)
	ldq $3,32($15)
	ldq $2,16($15)
	bis $3,$2,$2
	stq $2,0($1)
	ldq $1,48($15)
	lda $1,8($1)
	stq $1,48($15)
	ldq $1,24($15)
	stq $1,32($15)
	ldq $1,64($15)
	bge $1,$L7
$L6:
	ldq $1,64($15)
	lda $1,8($1)
	stq $1,64($15)
	br $31,$L8
$L9:
	ldq $1,64($15)
	lda $1,-1($1)
	stq $1,64($15)
	ldq $1,48($15)
	ldq $2,56($15)
	lda $3,1($2)
	ldq_u $2,0($2)
	extqh $2,$3,$2
	sra $2,56,$2
	bis $31,$2,$4
	ldq_u $3,0($1)
	mov $1,$2
	mskbl $3,$2,$3
	insbl $4,$2,$2
	bis $2,$3,$2
	stq_u $2,0($1)
	ldq $1,48($15)
	lda $1,1($1)
	stq $1,48($15)
	ldq $1,56($15)
	lda $1,1($1)
	stq $1,56($15)
$L8:
	ldq $1,64($15)
	bgt $1,$L9
$L10:
	mov $15,$30
	ldq $26,0($30)
	ldq $15,8($30)
	lda $30,80($30)
	ret $31,($26),1
	.end __memcpy_unaligned_up
	.align 2
	.ent __memcpy_aligned_up
$__memcpy_aligned_up..ng:
__memcpy_aligned_up:
	.frame $15,64,$26,0
	.mask 0x4008000,-64
	lda $30,-64($30)
	stq $26,0($30)
	stq $15,8($30)
	mov $30,$15
	.prologue 0
	stq $16,32($15)
	stq $17,40($15)
	stq $18,48($15)
	br $31,$L12
$L15:
	ldq $1,48($15)
	ble $1,$L20
$L13:
	ldq $1,48($15)
	lda $1,-1($1)
	stq $1,48($15)
	ldq $1,32($15)
	ldq $2,40($15)
	lda $3,1($2)
	ldq_u $2,0($2)
	extqh $2,$3,$2
	sra $2,56,$2
	bis $31,$2,$4
	ldq_u $3,0($1)
	mov $1,$2
	mskbl $3,$2,$3
	insbl $4,$2,$2
	bis $2,$3,$2
	stq_u $2,0($1)
	ldq $1,32($15)
	lda $1,1($1)
	stq $1,32($15)
	ldq $1,40($15)
	lda $1,1($1)
	stq $1,40($15)
$L12:
	ldq $1,32($15)
	and $1,7,$1
	bne $1,$L15
	ldq $1,48($15)
	lda $1,-8($1)
	stq $1,48($15)
	br $31,$L16
$L17:
	ldq $1,40($15)
	.set	macro
 # 123 "memcpy.c" 1
	ldq $1,0($1)
 # 0 "" 2
	.set	nomacro
	stq $1,16($15)
	ldq $1,48($15)
	lda $1,-8($1)
	stq $1,48($15)
	ldq $1,40($15)
	lda $1,8($1)
	stq $1,40($15)
	ldq $1,32($15)
	ldq $2,16($15)
	stq $2,0($1)
	ldq $1,32($15)
	lda $1,8($1)
	stq $1,32($15)
$L16:
	ldq $1,48($15)
	bge $1,$L17
	ldq $1,48($15)
	lda $1,8($1)
	stq $1,48($15)
	br $31,$L18
$L19:
	ldq $1,48($15)
	lda $1,-1($1)
	stq $1,48($15)
	ldq $1,32($15)
	ldq $2,40($15)
	lda $3,1($2)
	ldq_u $2,0($2)
	extqh $2,$3,$2
	sra $2,56,$2
	bis $31,$2,$4
	ldq_u $3,0($1)
	mov $1,$2
	mskbl $3,$2,$3
	insbl $4,$2,$2
	bis $2,$3,$2
	stq_u $2,0($1)
	ldq $1,32($15)
	lda $1,1($1)
	stq $1,32($15)
	ldq $1,40($15)
	lda $1,1($1)
	stq $1,40($15)
$L18:
	ldq $1,48($15)
	bgt $1,$L19
$L20:
	mov $15,$30
	ldq $26,0($30)
	ldq $15,8($30)
	lda $30,64($30)
	ret $31,($26),1
	.end __memcpy_aligned_up
	.align 2
	.globl memcpy
	.ent memcpy
memcpy:
	.frame $15,48,$26,0
	.mask 0x4008000,-48
	ldah $29,0($27)		!gpdisp!1
	lda $29,0($29)		!gpdisp!1
$memcpy..ng:
	lda $30,-48($30)
	stq $26,0($30)
	stq $15,8($30)
	mov $30,$15
	.prologue 1
	stq $16,16($15)
	stq $17,24($15)
	stq $18,32($15)
	ldq $2,16($15)
	ldq $1,24($15)
	xor $2,$1,$1
	and $1,7,$1
	bne $1,$L22
	ldq $3,16($15)
	ldq $2,24($15)
	ldq $1,32($15)
	mov $3,$16
	mov $2,$17
	mov $1,$18
	bsr $26,__memcpy_aligned_up		!samegp
	ldq $1,16($15)
	br $31,$L23
$L22:
	ldq $3,16($15)
	ldq $2,24($15)
	ldq $1,32($15)
	mov $3,$16
	mov $2,$17
	mov $1,$18
	bsr $26,__memcpy_unaligned_up		!samegp
	ldq $1,16($15)
$L23:
	mov $1,$0
	mov $15,$30
	ldq $26,0($30)
	ldq $15,8($30)
	lda $30,48($30)
	ret $31,($26),1
	.end memcpy
	.ident	"GCC: (GNU) 4.4.0"
	.section	.note.GNU-stack,"",@progbits
