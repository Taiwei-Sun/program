	.file	"test1.c"
	.section	.rodata
.LC0:
	.string	"./test.txt"
.LC1:
	.string	"open"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$1040, %rsp
	movl	$0, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	open
	movl	%eax, -8(%rbp)
	cmpl	$-1, -8(%rbp)
	jne	.L2
	movl	$.LC1, %edi
	call	perror
	movl	$1, %eax
	jmp	.L1
.L2:
	movl	$10, %edi
	call	putchar
	jmp	.L4
.L7:
	movl	$0, -4(%rbp)
	jmp	.L5
.L6:
	movl	-4(%rbp), %eax
	cltq
	movzbl	-1040(%rbp,%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	putchar
	addl	$1, -4(%rbp)
.L5:
	movl	-4(%rbp), %eax
	cltq
	cmpq	-16(%rbp), %rax
	jl	.L6
	movl	$10, %edi
	call	putchar
.L4:
	leaq	-1040(%rbp), %rcx
	movl	-8(%rbp), %eax
	movl	$1024, %edx
	movq	%rcx, %rsi
	movl	%eax, %edi
	movl	$0, %eax
	call	read
	cltq
	movq	%rax, -16(%rbp)
	cmpq	$0, -16(%rbp)
	jg	.L7
	movl	-8(%rbp), %eax
	movl	%eax, %edi
	movl	$0, %eax
	call	close
.L1:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-16)"
	.section	.note.GNU-stack,"",@progbits
