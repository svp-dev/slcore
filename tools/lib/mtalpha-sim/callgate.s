

	.text
	.align 4
	.globl _sl_callgate
	.ent _sl_callgate
_sl_callgate:
	.registers 0 0 31 0 0 31

	ldq $9, 0($0)  # load call protocol
	ldq $27, 8($0) # load callee function pointer into PV
	mov $1, $30   # set up stack pointer
	mov $1, $15   # set up frame pointer

	beq $9, $docall		; swch
	ldq $16, 32($0)
	ldt $f16, 32($0)

	subl $9, 1, $9
	beq $9, $docall		; swch
	ldq $17, 40($0)		
	ldt $f17, 40($0)

	subl $9, 1, $9
	beq $9, $docall		; swch
	ldq $18, 48($0)
	ldt $f18, 48($0)

	subl $9, 1, $9
	beq $9, $docall		; swch
	ldq $19, 56($0)
	ldt $f19, 56($0)

	subl $9, 1, $9
	beq $9, $docall		; swch
	ldq $20, 64($0)
	ldt $f20, 64($0)

	subl $9, 1, $9
	beq $9, $docall		; swch
	ldq $21, 72($0)
	ldt $f21, 72($0)

	subl $9, 1, $9
	beq $9, $docall		; swch

	# if we reach here, the call protocol is unsupported yet.
	nop
	nop
$die:	pal1d 0
	br $die			; swch
	.align 6

$docall:
	mov $0, $10 # save up for later
	mov $31, $11 # flush callee-save reg
	mov $31, $12 # flush callee-save reg
	mov $31, $13 # flush callee-save reg
	mov $31, $14 # flush callee-save reg
	cpys $f31, $f31, $f2 # flush callee-save reg
	cpys $f31, $f31, $f3 # flush callee-save reg
	cpys $f31, $f31, $f4 # flush callee-save reg
	cpys $f31, $f31, $f5 # flush callee-save reg
	cpys $f31, $f31, $f6 # flush callee-save reg
	cpys $f31, $f31, $f7 # flush callee-save reg
	cpys $f31, $f31, $f0 # init FP return reg

	# call function
	jsr $26,($27)		; swch

	# save return value to predefined location
	stq $0, 16($10)
	stt $f0, 24($10)		; end

	.end _sl_callgate
	
	