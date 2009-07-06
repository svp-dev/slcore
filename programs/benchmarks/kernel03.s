	.set noreorder		
	.set volatile		
	.set noat		
	.set nomacro		
	.arch ev6		
	.section	.rodata.str1.1,"aMS",@progbits,1		
$LC0: 
.ascii "***LIVERMORE WARNING: Specified problem size exceed" 
	.ascii "s 8300!***\12\12\0"		
$LC1: 
	.ascii "--------------------------\12\0"		
$LC2: 
.ascii "   LIVERMORE LOOPS:  \12\0" 
$LC3: 
	.ascii "   *** kernel3 *** \12\12\0"		
$LC4: 
	.ascii " Using Initialised Data. \12\0"		
$LC5: 
.ascii " Inner Loops: \0" 
$LC6: 
.ascii "\12 Output Loops: \0" 
$LC7: 
.ascii "\12 Specified Block Size: \0" 
$LC8: 
.ascii "\12\12 Cycles: \0" 
$LC9: 
	.ascii "\12--------------------------\12\0"		
	.text		
	.align 2		
	.align 4		
	.globl t_main		
	.ent t_main		
t_main: 
	.registers 1 0 7 0 0 0		
# MT: compressed integer locals from 19 to 7
# MT: compressed FP locals from 19 to 0
# MT: special "gp" renamed from $l17 to $l6
	ldah $l6, 0($g0) !gpdisp!1		
	lda $l6, 0($l6) !gpdisp!1		
$t_main..ng: 
	.prologue 1		
	.set	macro		
# 117 "/tmp/slc.Tnt29130/kernel03.i" 1

# 0 "" 2
	.set	nomacro		
	ldq $l1,__slr_base($l6)		!literal		
	ldah $l0,inner($l6)		!gprelhigh		
	lda $l0,inner($l0)		!gprellow		
	ldq $l1,0($l1)		
	swch		# MT: prev insn may read result of long latency op ($l1)
	ldq $l2,0($l1)		
	swch		# MT: prev insn may read result of long latency op ($l1)
	addq $l1,$l2,$l1		
	swch		# MT: prev insn may read result of long latency op ($l2,$l2)
	ldl $l1,0($l1)		
	beq $l1,$L2		
	swch		# MT: swch after branch
	zapnot $l1,15,$l3		
	lda $l2,8299($31)		
	cmpule $l3,$l2,$l2		
	beq $l2,$L3		
	swch		# MT: swch after branch
	stl $l1,12($l0)		
$L2: 
	.set	macro		
# 142 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l3, 1, 3, 1, 1		# CREATE __child2
# 0 "" 2
	.set	nomacro		
	ldah $l1,$LC1($l6)		!gprelhigh		
	ldq $l2,svp_io_puts($l6)		!literal		
	lda $l1,$LC1($l1)		!gprellow		
	.set	macro		
# 145 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l3, 1" ignored (uses default) CREATE __child2
# 0 "" 2
# 146 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l3, 0" ignored (uses default) CREATE __child2
# 0 "" 2
# 147 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setlimit $l3, 1" ignored (uses default) CREATE __child2
# 0 "" 2
# 148 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l3, 1" ignored (uses default) CREATE __child2
# 0 "" 2
# 149 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setblock $l3, 0" ignored (uses default) CREATE __child2
# 0 "" 2
# 151 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l3, 0($l2)		# CREATE __child2
	swch		# MT: prev insn may read result of long latency op ($l2,$l3,$l3,$l2)
# 0 "" 2
# 152 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l3, $31, $31		# SYNC __child2
	swch		# MT: prev insn may read result of long latency op ($l3)
# 0 "" 2
# 155 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l3, 1, 3, 1, 1		# CREATE __child3
# 0 "" 2
	.set	nomacro		
	ldah $l1,$LC2($l6)		!gprelhigh		
	lda $l1,$LC2($l1)		!gprellow		
	.set	macro		
# 158 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l3, 1" ignored (uses default) CREATE __child3
# 0 "" 2
# 159 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l3, 0" ignored (uses default) CREATE __child3
# 0 "" 2
# 160 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setlimit $l3, 1" ignored (uses default) CREATE __child3
# 0 "" 2
# 161 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l3, 1" ignored (uses default) CREATE __child3
# 0 "" 2
# 162 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setblock $l3, 0" ignored (uses default) CREATE __child3
# 0 "" 2
# 164 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l3, 0($l2)		# CREATE __child3
	swch		# MT: prev insn may read result of long latency op ($l3)
# 0 "" 2
# 165 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l3, $31, $31		# SYNC __child3
	swch		# MT: prev insn may read result of long latency op ($l3)
# 0 "" 2
# 168 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l3, 1, 3, 1, 1		# CREATE __child4
# 0 "" 2
	.set	nomacro		
	ldah $l1,$LC3($l6)		!gprelhigh		
	lda $l1,$LC3($l1)		!gprellow		
	.set	macro		
# 171 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l3, 1" ignored (uses default) CREATE __child4
# 0 "" 2
# 172 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l3, 0" ignored (uses default) CREATE __child4
# 0 "" 2
# 173 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setlimit $l3, 1" ignored (uses default) CREATE __child4
# 0 "" 2
# 174 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l3, 1" ignored (uses default) CREATE __child4
# 0 "" 2
# 175 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setblock $l3, 0" ignored (uses default) CREATE __child4
# 0 "" 2
# 177 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l3, 0($l2)		# CREATE __child4
	swch		# MT: prev insn may read result of long latency op ($l3)
# 0 "" 2
# 178 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l3, $31, $31		# SYNC __child4
	swch		# MT: prev insn may read result of long latency op ($l3)
# 0 "" 2
# 181 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l3, 1, 3, 1, 1		# CREATE __child5
# 0 "" 2
	.set	nomacro		
	ldah $l1,$LC4($l6)		!gprelhigh		
	lda $l1,$LC4($l1)		!gprellow		
	.set	macro		
# 184 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l3, 1" ignored (uses default) CREATE __child5
# 0 "" 2
# 185 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l3, 0" ignored (uses default) CREATE __child5
# 0 "" 2
# 186 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setlimit $l3, 1" ignored (uses default) CREATE __child5
# 0 "" 2
# 187 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l3, 1" ignored (uses default) CREATE __child5
# 0 "" 2
# 188 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setblock $l3, 0" ignored (uses default) CREATE __child5
# 0 "" 2
# 190 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l3, 0($l2)		# CREATE __child5
	swch		# MT: prev insn may read result of long latency op ($l3)
# 0 "" 2
# 191 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l3, $31, $31		# SYNC __child5
	swch		# MT: prev insn may read result of long latency op ($l3)
# 0 "" 2
# 194 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l3, 1, 3, 1, 1		# CREATE __child6
# 0 "" 2
	.set	nomacro		
	ldah $l1,$LC5($l6)		!gprelhigh		
	lda $l1,$LC5($l1)		!gprellow		
	.set	macro		
# 197 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l3, 1" ignored (uses default) CREATE __child6
# 0 "" 2
# 198 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l3, 0" ignored (uses default) CREATE __child6
# 0 "" 2
# 199 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setlimit $l3, 1" ignored (uses default) CREATE __child6
# 0 "" 2
# 200 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l3, 1" ignored (uses default) CREATE __child6
# 0 "" 2
# 201 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setblock $l3, 0" ignored (uses default) CREATE __child6
# 0 "" 2
# 203 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l3, 0($l2)		# CREATE __child6
	swch		# MT: prev insn may read result of long latency op ($l3)
# 0 "" 2
# 204 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l3, $31, $31		# SYNC __child6
	swch		# MT: prev insn may read result of long latency op ($l3)
# 0 "" 2
# 207 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l4, 1, 4, 1, 1		# CREATE __child7
# 0 "" 2
	.set	nomacro		
	ldl $l1,12($l0)		
	swch		# MT: prev insn may read result of long latency op ($l0)
	lda $l2,10($31)		
	ldq $l3,svp_io_putun($l6)		!literal		
	.set	macro		
# 211 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l4, 1" ignored (uses default) CREATE __child7
# 0 "" 2
# 212 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l4, 0" ignored (uses default) CREATE __child7
# 0 "" 2
	.set	nomacro		
	zapnot $l1,15,$l1		
	swch		# MT: prev insn may read result of long latency op ($l1)
	.set	macro		
# 213 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setlimit $l4, 1" ignored (uses default) CREATE __child7
# 0 "" 2
# 214 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l4, 1" ignored (uses default) CREATE __child7
# 0 "" 2
# 215 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setblock $l4, 0" ignored (uses default) CREATE __child7
# 0 "" 2
# 217 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l4, 0($l3)		# CREATE __child7
	swch		# MT: prev insn may read result of long latency op ($l4,$l4,$l3)
# 0 "" 2
# 218 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l4, $31, $31		# SYNC __child7
	swch		# MT: prev insn may read result of long latency op ($l4)
# 0 "" 2
# 221 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l3, 1, 3, 1, 1		# CREATE __child8
# 0 "" 2
	.set	nomacro		
	ldah $l1,$LC6($l6)		!gprelhigh		
	ldq $l2,svp_io_puts($l6)		!literal		
	lda $l1,$LC6($l1)		!gprellow		
	.set	macro		
# 224 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l3, 1" ignored (uses default) CREATE __child8
# 0 "" 2
# 225 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l3, 0" ignored (uses default) CREATE __child8
# 0 "" 2
# 226 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setlimit $l3, 1" ignored (uses default) CREATE __child8
# 0 "" 2
# 227 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l3, 1" ignored (uses default) CREATE __child8
# 0 "" 2
# 228 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setblock $l3, 0" ignored (uses default) CREATE __child8
# 0 "" 2
# 230 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l3, 0($l2)		# CREATE __child8
	swch		# MT: prev insn may read result of long latency op ($l3,$l2)
# 0 "" 2
# 231 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l3, $31, $31		# SYNC __child8
	swch		# MT: prev insn may read result of long latency op ($l3)
# 0 "" 2
# 234 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l4, 1, 4, 1, 1		# CREATE __child9
# 0 "" 2
	.set	nomacro		
	ldah $l0,outer($l6)		!gprelhigh		
	lda $l2,10($31)		
	ldq $l3,svp_io_putun($l6)		!literal		
	lda $l0,outer($l0)		!gprellow		
	ldl $l1,12($l0)		
	.set	macro		
# 238 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l4, 1" ignored (uses default) CREATE __child9
# 0 "" 2
# 239 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l4, 0" ignored (uses default) CREATE __child9
# 0 "" 2
	.set	nomacro		
	zapnot $l1,15,$l1		
	swch		# MT: prev insn may read result of long latency op ($l1)
	.set	macro		
# 240 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setlimit $l4, 1" ignored (uses default) CREATE __child9
# 0 "" 2
# 241 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l4, 1" ignored (uses default) CREATE __child9
# 0 "" 2
# 242 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setblock $l4, 0" ignored (uses default) CREATE __child9
# 0 "" 2
# 244 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l4, 0($l3)		# CREATE __child9
	swch		# MT: prev insn may read result of long latency op ($l4,$l3)
# 0 "" 2
# 245 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l4, $31, $31		# SYNC __child9
	swch		# MT: prev insn may read result of long latency op ($l4)
# 0 "" 2
# 248 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l3, 1, 3, 1, 1		# CREATE __child10
# 0 "" 2
	.set	nomacro		
	ldah $l1,$LC7($l6)		!gprelhigh		
	ldq $l2,svp_io_puts($l6)		!literal		
	lda $l1,$LC7($l1)		!gprellow		
	.set	macro		
# 251 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l3, 1" ignored (uses default) CREATE __child10
# 0 "" 2
# 252 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l3, 0" ignored (uses default) CREATE __child10
# 0 "" 2
# 253 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setlimit $l3, 1" ignored (uses default) CREATE __child10
# 0 "" 2
# 254 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l3, 1" ignored (uses default) CREATE __child10
# 0 "" 2
# 255 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setblock $l3, 0" ignored (uses default) CREATE __child10
# 0 "" 2
# 257 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l3, 0($l2)		# CREATE __child10
	swch		# MT: prev insn may read result of long latency op ($l3,$l2)
# 0 "" 2
# 258 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l3, $31, $31		# SYNC __child10
	swch		# MT: prev insn may read result of long latency op ($l3)
# 0 "" 2
# 261 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l4, 1, 4, 1, 1		# CREATE __child11
# 0 "" 2
	.set	nomacro		
	ldah $l1,blocksize+12($l6)		!gprelhigh		
	lda $l2,10($31)		
	ldq $l3,svp_io_putun($l6)		!literal		
	ldl $l1,blocksize+12($l1)		!gprellow		
	.set	macro		
# 265 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l4, 1" ignored (uses default) CREATE __child11
# 0 "" 2
# 266 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l4, 0" ignored (uses default) CREATE __child11
# 0 "" 2
	.set	nomacro		
	zapnot $l1,15,$l1		
	swch		# MT: prev insn may read result of long latency op ($l1)
	.set	macro		
# 267 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setlimit $l4, 1" ignored (uses default) CREATE __child11
# 0 "" 2
# 268 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l4, 1" ignored (uses default) CREATE __child11
# 0 "" 2
# 269 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setblock $l4, 0" ignored (uses default) CREATE __child11
# 0 "" 2
# 271 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l4, 0($l3)		# CREATE __child11
	swch		# MT: prev insn may read result of long latency op ($l4,$l3)
# 0 "" 2
# 272 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l4, $31, $31		# SYNC __child11
	swch		# MT: prev insn may read result of long latency op ($l4)
# 0 "" 2
	.set	nomacro		
	ldl $l1,12($l0)		
	mov $31,$l5		
	mov $31,$l3		
	beq $l1,$L5		
	swch		# MT: swch after branch
	.align 4		
$L9: 
	.set	macro		
# 276 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l2, 1, 2, 1, 1		# CREATE __child12
# 0 "" 2
# 278 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l2, 1" ignored (uses default) CREATE __child12
# 0 "" 2
	.set	nomacro		
	ldq $l1,initialiser($l6)		!literal		
	.set	macro		
# 279 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l2, 0" ignored (uses default) CREATE __child12
# 0 "" 2
# 280 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setlimit $l2, 1" ignored (uses default) CREATE __child12
# 0 "" 2
# 281 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l2, 1" ignored (uses default) CREATE __child12
# 0 "" 2
# 282 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setblock $l2, 0" ignored (uses default) CREATE __child12
# 0 "" 2
# 284 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l2, 0($l1)		# CREATE __child12
	swch		# MT: prev insn may read result of long latency op ($l1,$l2,$l2,$l1)
# 0 "" 2
# 286 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l2, $31, $31		# SYNC __child12
	swch		# MT: prev insn may read result of long latency op ($l2)
# 0 "" 2
# 289 "/tmp/slc.Tnt29130/kernel03.i" 1
	rpcc $l4		
# 0 "" 2
# 292 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l2, 1, 2, 1, 1		# CREATE __child13
# 0 "" 2
# 294 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l2, 1" ignored (uses default) CREATE __child13
# 0 "" 2
	.set	nomacro		
	ldq $l1,kernel3($l6)		!literal		
	.set	macro		
# 295 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l2, 0" ignored (uses default) CREATE __child13
# 0 "" 2
# 296 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setlimit $l2, 1" ignored (uses default) CREATE __child13
# 0 "" 2
# 297 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l2, 1" ignored (uses default) CREATE __child13
# 0 "" 2
# 298 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setblock $l2, 0" ignored (uses default) CREATE __child13
# 0 "" 2
# 300 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l2, 0($l1)		# CREATE __child13
	swch		# MT: prev insn may read result of long latency op ($l2,$l1)
# 0 "" 2
# 302 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l2, $31, $31		# SYNC __child13
	swch		# MT: prev insn may read result of long latency op ($l2)
# 0 "" 2
# 305 "/tmp/slc.Tnt29130/kernel03.i" 1
	rpcc $l2		
# 0 "" 2
	.set	nomacro		
	ldl $l1,12($l0)		
	swch		# MT: prev insn may read result of long latency op ($l0)
	addl $l3,1,$l3		
	swch		# MT: prev insn may read result of long latency op ($l3)
	subq $l2,$l4,$l4		
	zapnot $l3,15,$l2		
	addq $l5,$l4,$l5		
	swch		# MT: prev insn may read result of long latency op ($l5)
	zapnot $l1,15,$l1		
	swch		# MT: prev insn may read result of long latency op ($l1)
	cmpule $l1,$l2,$l1		
	beq $l1,$L9		
	swch		# MT: swch after branch
$L5: 
	.set	macro		
# 310 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l3, 1, 3, 1, 1		# CREATE __child14
# 0 "" 2
	.set	nomacro		
	ldah $l1,$LC8($l6)		!gprelhigh		
	ldq $l2,svp_io_puts($l6)		!literal		
	lda $l1,$LC8($l1)		!gprellow		
	.set	macro		
# 313 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l3, 1" ignored (uses default) CREATE __child14
# 0 "" 2
# 314 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l3, 0" ignored (uses default) CREATE __child14
# 0 "" 2
# 315 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setlimit $l3, 1" ignored (uses default) CREATE __child14
# 0 "" 2
# 316 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l3, 1" ignored (uses default) CREATE __child14
# 0 "" 2
# 317 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setblock $l3, 0" ignored (uses default) CREATE __child14
# 0 "" 2
# 319 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l3, 0($l2)		# CREATE __child14
	swch		# MT: prev insn may read result of long latency op ($l2,$l3,$l3,$l2)
# 0 "" 2
# 320 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l3, $31, $31		# SYNC __child14
	swch		# MT: prev insn may read result of long latency op ($l3)
# 0 "" 2
# 323 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l4, 1, 4, 1, 1		# CREATE __child15
# 0 "" 2
	.set	nomacro		
	lda $l2,10($31)		
	mov $l5,$l1		
	swch		# MT: prev insn may read result of long latency op ($l5)
	ldq $l3,svp_io_putun($l6)		!literal		
	.set	macro		
# 327 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l4, 1" ignored (uses default) CREATE __child15
# 0 "" 2
# 328 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l4, 0" ignored (uses default) CREATE __child15
# 0 "" 2
# 329 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setlimit $l4, 1" ignored (uses default) CREATE __child15
# 0 "" 2
# 330 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l4, 1" ignored (uses default) CREATE __child15
# 0 "" 2
# 331 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setblock $l4, 0" ignored (uses default) CREATE __child15
# 0 "" 2
# 333 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l4, 0($l3)		# CREATE __child15
	swch		# MT: prev insn may read result of long latency op ($l4,$l4,$l3)
# 0 "" 2
# 334 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l4, $31, $31		# SYNC __child15
	swch		# MT: prev insn may read result of long latency op ($l4)
# 0 "" 2
# 337 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l3, 1, 3, 1, 1		# CREATE __child16
# 0 "" 2
	.set	nomacro		
	ldah $l1,$LC9($l6)		!gprelhigh		
	ldq $l2,svp_io_puts($l6)		!literal		
	lda $l1,$LC9($l1)		!gprellow		
	.set	macro		
# 340 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l3, 1" ignored (uses default) CREATE __child16
# 0 "" 2
# 341 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l3, 0" ignored (uses default) CREATE __child16
# 0 "" 2
# 342 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setlimit $l3, 1" ignored (uses default) CREATE __child16
# 0 "" 2
# 343 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l3, 1" ignored (uses default) CREATE __child16
# 0 "" 2
# 344 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setblock $l3, 0" ignored (uses default) CREATE __child16
# 0 "" 2
# 346 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l3, 0($l2)		# CREATE __child16
	swch		# MT: prev insn may read result of long latency op ($l3,$l2)
# 0 "" 2
# 347 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l3, $31, $31		# SYNC __child16
	swch		# MT: prev insn may read result of long latency op ($l3)
# 0 "" 2
	.set	nomacro		
	nop		# END_THREAD
	end		# END_THREAD
	.align 4		
$L3: 
	.set	macro		
# 128 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l3, 1, 3, 1, 1		# CREATE __child1
# 0 "" 2
	.set	nomacro		
	ldah $l1,$LC0($l6)		!gprelhigh		
	ldq $l2,svp_io_puts($l6)		!literal		
	lda $l1,$LC0($l1)		!gprellow		
	.set	macro		
# 131 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l3, 1" ignored (uses default) CREATE __child1
# 0 "" 2
# 132 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l3, 0" ignored (uses default) CREATE __child1
# 0 "" 2
# 133 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setlimit $l3, 1" ignored (uses default) CREATE __child1
# 0 "" 2
# 134 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l3, 1" ignored (uses default) CREATE __child1
# 0 "" 2
# 135 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setblock $l3, 0" ignored (uses default) CREATE __child1
# 0 "" 2
# 137 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l3, 0($l2)		# CREATE __child1
	swch		# MT: prev insn may read result of long latency op ($l2,$l3,$l3,$l2)
# 0 "" 2
# 138 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l3, $31, $31		# SYNC __child1
	swch		# MT: prev insn may read result of long latency op ($l3)
# 0 "" 2
	.set	nomacro		
	br $31,$L2		
	swch		# MT: swch after branch
	.end t_main		
	.align 2		
	.align 4		
	.globl simpleinit		
	.ent simpleinit		
simpleinit: 
	.registers 0 0 2 0 0 0		
# MT: compressed integer locals from 19 to 2
# MT: compressed integer globals from 2 to 0
# MT: compressed FP locals from 19 to 0
$simpleinit..ng: 
	.prologue 0		
	.set	macro		
# 366 "/tmp/slc.Tnt29130/kernel03.i" 1

# 0 "" 2
	.set	nomacro		
	nop		# END_THREAD
	end		# END_THREAD
	.end simpleinit		
	.align 2		
	.align 4		
	.globl initialiser		
	.ent initialiser		
initialiser: 
	.registers 1 0 5 0 0 0		
# MT: compressed integer locals from 19 to 5
# MT: compressed FP locals from 19 to 0
# MT: special "gp" renamed from $l17 to $l4
	ldah $l4, 0($g0) !gpdisp!2		
	lda $l4, 0($l4) !gpdisp!2		
$initialiser..ng: 
	.prologue 1		
	.set	macro		
# 373 "/tmp/slc.Tnt29130/kernel03.i" 1

# 0 "" 2
# 376 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l3, 1, 3, 1, 1		# CREATE __child17
# 0 "" 2
	.set	nomacro		
	lda $l0,8300($31)		
	ldq $l1,data+66400($l4)		!literal		
	ldq $l2,simpleinit($l4)		!literal		
	.set	macro		
# 379 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l3, 1" ignored (uses default) CREATE __child17
# 0 "" 2
# 380 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l3, 0" ignored (uses default) CREATE __child17
# 0 "" 2
# 381 "/tmp/slc.Tnt29130/kernel03.i" 1
	setlimit $l3, $l0		# CREATE __child17
	swch		# MT: prev insn may read result of long latency op ($l3,$l3)
# 0 "" 2
# 382 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l3, 1" ignored (uses default) CREATE __child17
# 0 "" 2
# 383 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setblock $l3, 0" ignored (uses default) CREATE __child17
# 0 "" 2
# 385 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l3, 0($l2)		# CREATE __child17
	swch		# MT: prev insn may read result of long latency op ($l2,$l2)
# 0 "" 2
# 387 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l3, $31, $31		# SYNC __child17
	swch		# MT: prev insn may read result of long latency op ($l3)
# 0 "" 2
	.set	nomacro		
	nop		# END_THREAD
	end		# END_THREAD
	.end initialiser		
	.align 2		
	.align 4		
	.globl innerk3		
	.ent innerk3		
innerk3: 
	.registers 2 0 2 0 1 2		
# MT: compressed integer locals from 19 to 2
# MT: compressed integer globals from 3 to 2
# MT: compressed FP locals from 19 to 2
$innerk3..ng: 
	.prologue 0		
	.set	macro		
# 390 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: index starts in $l0 (must be =$l0)
# 0 "" 2
# 390 "/tmp/slc.Tnt29130/kernel03.i" 1

# 0 "" 2
# 396 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: start write shared ql ($sf0) <- __sl_getshp(ql) + (__sl_glparm_zl[iteration]*__sl_glparm_xl[iteration]) (and clobber incoming $df0)
# 0 "" 2
# 396 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: read shared ql ($df0)
# 0 "" 2
	.set	nomacro		
	s8addq $l0,0,$l0		
	addq $g0,$l0,$l1		
	addq $g1,$l0,$l0		
	ldt $lf1,0($l0)		
	ldt $lf0,0($l1)		
	mult $lf1,$lf0,$lf0		
	swch		# MT: prev insn may read result of long latency op ($lf0,$lf1,$lf1,$lf0)
	addt $lf0,$df0,$sf0		
	swch		# MT: prev insn may read result of long latency op ($df0,$lf0)
	.set	macro		
# 396 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: end write shared ql ($sf0)
# 0 "" 2
	.set	nomacro		
	nop		# END_THREAD
	end		# END_THREAD
	.end innerk3		
	.align 2		
	.align 4		
	.globl kernel3		
	.ent kernel3		
kernel3: 
	.registers 1 0 7 0 0 2		
# MT: compressed integer locals from 19 to 7
# MT: compressed FP locals from 19 to 2
# MT: special "gp" renamed from $l17 to $l6
	ldah $l6, 0($g0) !gpdisp!3		
	lda $l6, 0($l6) !gpdisp!3		
$kernel3..ng: 
	.prologue 1		
	.set	macro		
# 399 "/tmp/slc.Tnt29130/kernel03.i" 1

# 0 "" 2
# 402 "/tmp/slc.Tnt29130/kernel03.i" 1
	allocate $l4, 1, 4, 1, 1		# CREATE __child18
# 0 "" 2
	.set	nomacro		
	ldq $l0,data($l6)		!literal		
	ldah $l1,inner+12($l6)		!gprelhigh		
	cpys $f31,$f31,$lf1		
	ldq $l3,innerk3($l6)		!literal		
	ldl $l5,inner+12($l1)		!gprellow		
	.set	macro		
# 407 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setplace $l4, 1" ignored (uses default) CREATE __child18
# 0 "" 2
	.set	nomacro		
	ldah $l1,3($l0)		
	swch		# MT: prev insn may read result of long latency op ($l0)
	ldah $l2,1($l0)		
	lda $l1,2592($l1)		
	lda $l2,864($l2)		
	.set	macro		
# 408 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstart $l4, 0" ignored (uses default) CREATE __child18
# 0 "" 2
# 409 "/tmp/slc.Tnt29130/kernel03.i" 1
	setlimit $l4, $l5		# CREATE __child18
	swch		# MT: prev insn may read result of long latency op ($l4,$l5,$l4,$l5)
# 0 "" 2
# 410 "/tmp/slc.Tnt29130/kernel03.i" 1
# MT: "setstep $l4, 1" ignored (uses default) CREATE __child18
# 0 "" 2
# 411 "/tmp/slc.Tnt29130/kernel03.i" 1
	setblock $l4, 2		# CREATE __child18
# 0 "" 2
# 413 "/tmp/slc.Tnt29130/kernel03.i" 1
	crei $l4, 0($l3)		# CREATE __child18
	swch		# MT: prev insn may read result of long latency op ($l3,$l3)
# 0 "" 2
# 415 "/tmp/slc.Tnt29130/kernel03.i" 1
	bis $l4, $31, $31		# SYNC __child18
	swch		# MT: prev insn may read result of long latency op ($l4)
# 0 "" 2
	.set	nomacro		
	ldah $l0,16853($l0)		
	stt $lf1,19696($l0)		
	nop		# END_THREAD
	end		# END_THREAD
	.end kernel3		
	.globl inner		
	.data		
	.align 2		
	.type	inner, @object		
	.size	inner, 100		
inner: 
	.long	0		
	.long	7000		
	.long	7000		
	.long	7000		
	.long	7000		
	.long	7000		
	.long	500		
	.long	7000		
	.long	7000		
	.long	7000		
	.long	7000		
	.long	7000		
	.long	7000		
	.long	10		
	.long	10		
	.long	10		
	.long	10		
	.long	10		
	.long	10		
	.long	10		
	.long	10		
	.long	10		
	.long	10		
	.long	10		
	.long	10		
	.globl blocksize		
	.align 2		
	.type	blocksize, @object		
	.size	blocksize, 100		
blocksize: 
	.long	999		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.long	0		
	.globl outer		
	.align 2		
	.type	outer, @object		
	.size	outer, 100		
outer: 
	.long	0		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.long	1		
	.globl seeds		
	.align 3		
	.type	seeds, @object		
	.size	seeds, 64		
seeds: 
	.long	343597384		
	.long	1085675662		
	.long	515396076		
	.long	1085968837		
	.long	3435973837		
	.long	1086791948		
	.long	3435973837		
	.long	1087769740		
	.long	3229815407		
	.long	1074340298		
	.long	2755651017		
	.long	1074118367		
	.long	3985729651		
	.long	1070940028		
	.long	2233382994		
	.long	1077961195		
	.globl seedmask		
	.section	.sdata,"aws",@progbits		
	.align 3		
	.type	seedmask, @object		
	.size	seedmask, 8		
seedmask: 
	.quad	7		
	.globl __slr_argdef		
	.section	.rodata		
$LC10: 
.ascii "\0slr_arg:PSIZE:unsigned int:livermore problem size\0" 
	.section	.sdata		
	.align 3		
	.type	__slr_argdef, @object		
	.size	__slr_argdef, 8		
__slr_argdef: 
	.quad	$LC10		
	.comm	data,1104498000,8		
.ident	"GCC: (GNU) 4.4.0" 
	.section	.note.GNU-stack,"",@progbits		
