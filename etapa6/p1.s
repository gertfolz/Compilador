	.file	"t1.c"
	.text
	.section	.rodata

# STRING
.printIntStr: 
	.string	"%d\n"
	.text

# TAC_BEGINFUN
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp

#	TAC_PRINTINT
	movl	_a(%rip), %esi
	leaq	.printIntStr(%rip), %rdi
	call	printf@PLT

# TAC_ENDFUN
	popq	%rbp
	ret

.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits

	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5


# SEÇAO DE DADOS
	.data
	.type	_a, @object
	.size	_a, 4
_a:
	.long	6

0:
	.string	 "GNU"
1:
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
