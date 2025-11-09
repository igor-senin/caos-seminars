	.file	"race.c"
	.intel_syntax noprefix
	.text
	.globl	counter
	.bss
	.align 8
	.type	counter, @object
	.size	counter, 8
counter:
	.zero	8
	.text
	.globl	thread_routine
	.type	thread_routine, @function
thread_routine:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR [rbp-24], rdi
	mov	QWORD PTR [rbp-8], 0
	jmp	.L2
.L3:
	mov	rax, QWORD PTR counter[rip]
	add	rax, 1
	mov	QWORD PTR counter[rip], rax
	add	QWORD PTR [rbp-8], 1
.L2:
	cmp	QWORD PTR [rbp-8], 999999
	jbe	.L3
	mov	eax, 0
	pop	rbp
	ret
	.size	thread_routine, .-thread_routine
	.section	.rodata
.LC0:
	.string	"%ld\n"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	lea	rax, [rbp-16]
	mov	ecx, 0
	mov	edx, OFFSET FLAT:thread_routine
	mov	esi, 0
	mov	rdi, rax
	call	pthread_create
	lea	rax, [rbp-16]
	add	rax, 8
	mov	ecx, 0
	mov	edx, OFFSET FLAT:thread_routine
	mov	esi, 0
	mov	rdi, rax
	call	pthread_create
	mov	rax, QWORD PTR [rbp-16]
	mov	esi, 0
	mov	rdi, rax
	call	pthread_join
	mov	rax, QWORD PTR [rbp-8]
	mov	esi, 0
	mov	rdi, rax
	call	pthread_join
	mov	rax, QWORD PTR counter[rip]
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC0
	mov	eax, 0
	call	printf
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (SUSE Linux) 14.2.0"
	.section	.note.GNU-stack,"",@progbits
