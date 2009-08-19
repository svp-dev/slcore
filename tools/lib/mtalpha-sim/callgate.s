

	.text
	.align 4
	.globl __sl_callgate
	.ent __sl_callgate
__sl_callgate:
	.registers 0 0 31 0 0 31
	mov $l0, $30   # SP(parent)
	ldfp $l1       # TLSP

	ldq $9, -8($30)  # load call protocol
	ldq $27, -16($30) # load PV

	mov $30, $15   # set up frame pointer

	beq $9, $docall
	ldq $16, -24($0)
	ldt $f16, -32($0)

	subl $9, 1, $9
	beq $9, $docall
	ldq $17, -40($0)		
	ldt $f17, -48($0)

	subl $9, 1, $9
	beq $9, $docall
	ldq $18, -56($0)
	ldt $f18, -64($0)

	subl $9, 1, $9
	beq $9, $docall
	ldq $19, -72($0)
	ldt $f19, -80($0)

	subl $9, 1, $9
	beq $9, $docall
	ldq $20, -88($0)
	ldt $f20, -96($0)

	subl $9, 1, $9
	beq $9, $docall
	ldq $21, -104($0)
	ldt $f21, -112($0)

$docall:
	mov $31, $10 # flush callee-save reg
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
	cpys $f31, $f31, $f8 # flush callee-save reg
	cpys $f31, $f31, $f9 # flush callee-save reg
	cpys $f31, $f31, $f0 # init FP return reg

	# call function
	jsr $26,($27)

	# save return value(s)
	stq $0, -8($30)
	stt $f0, -16($30)
	end	

	.end __sl_callgate
	
	