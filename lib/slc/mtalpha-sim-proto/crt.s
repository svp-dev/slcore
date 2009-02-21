	.ent _start
	.globl _start
_start:
	ldah $29, 0($27)	!gpdisp!1
	lda  $29, 0($29)	!gpdisp!1
        ldq $27,t_main($29)               !literal!2
        jsr $26,($27),0         !lituse_jsr!2
	swch
        nop
        end
	.end _start

	.section .rodata
	.ascii "\0sgr_runner:mtalpha-sim-proto:\0"
