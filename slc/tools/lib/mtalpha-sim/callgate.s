

	.text
	.align 4
	.globl __sl_callgate
	.ent __sl_callgate
__sl_callgate:
	.registers 0 0 31 0 0 31
	mov $0, $30   # SP(parent)
	ldfp $1       # TLSP

	ldq $9, -8($30)  # load call protocol
	ldq $27, -16($30) # load PV

	mov $30, $15   # set up frame pointer
	clr $10 # flush callee-save reg
	clr $11 # flush callee-save reg
	clr $12 # flush callee-save reg
	clr $13 # flush callee-save reg
	clr $14 # flush callee-save reg
	fclr $f2 # flush callee-save reg
	fclr $f3 # flush callee-save reg
	fclr $f4 # flush callee-save reg
	fclr $f5 # flush callee-save reg
	fclr $f6 # flush callee-save reg
	fclr $f7 # flush callee-save reg
	fclr $f8 # flush callee-save reg
	fclr $f9 # flush callee-save reg
	fclr $f0 # init FP return reg

	beq $9, $docall_16
	ldq $16, -24($0)
	ldt $f16, -32($0)

	subl $9, 1, $9
	beq $9, $docall_17
	ldq $17, -40($0)		
	ldt $f17, -48($0)

	subl $9, 1, $9
	beq $9, $docall_18
	ldq $18, -56($0)
	ldt $f18, -64($0)

	subl $9, 1, $9
	beq $9, $docall_19
	ldq $19, -72($0)
	ldt $f19, -80($0)

	subl $9, 1, $9
	beq $9, $docall_20
	ldq $20, -88($0)
	ldt $f20, -96($0)

	subl $9, 1, $9
	beq $9, $docall_21
	ldq $21, -104($0)
	ldt $f21, -112($0)
	br $docall

$docall_16:
	clr $16
	fclr $f16	
$docall_17:
	clr $17
	fclr $f17
$docall_18:
	clr $18
	fclr $f18
$docall_19:
	clr $19
	fclr $f19
$docall_20:
	clr $20
	fclr $f20
$docall_21:
	clr $21
	fclr $f21
$docall:
	# call function
	jsr $26,($27)

	# save return value(s)
	stq $0, -8($30)
	stt $f0, -16($30)
	end	

	.end __sl_callgate
	
	