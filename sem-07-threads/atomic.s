	.file	"atomic.c"
	.intel_syntax noprefix
	.text
	.globl	thread_routine
	.type	thread_routine, @function
thread_routine:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR [rbp-24], rdi
	mov	rax, QWORD PTR [rbp-24]
	mov	QWORD PTR [rbp-16], rax
	mov	QWORD PTR [rbp-8], 0
	jmp	.L2
.L3:
	mov	rax, QWORD PTR [rbp-16]
	mov	rax, QWORD PTR [rax+8]
	lock add	DWORD PTR [rax], 1
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
	.string	"shared counter = %d\n"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 304
	mov	DWORD PTR [rbp-292], 0
	mov	QWORD PTR [rbp-8], 0
	jmp	.L6
.L7:
	mov	rax, QWORD PTR [rbp-8]
	sal	rax, 4
	add	rax, rbp
	lea	rdx, [rax-280]
	lea	rax, [rbp-292]
	mov	QWORD PTR [rdx], rax
	lea	rax, [rbp-288]
	mov	rdx, QWORD PTR [rbp-8]
	sal	rdx, 4
	lea	rdi, [rax+rdx]
	lea	rax, [rbp-288]
	mov	rcx, rax
	mov	edx, OFFSET FLAT:thread_routine
	mov	esi, 0
	call	pthread_create
	add	QWORD PTR [rbp-8], 1
.L6:
	cmp	QWORD PTR [rbp-8], 15
	jbe	.L7
	mov	QWORD PTR [rbp-16], 0
	jmp	.L8
.L9:
	mov	rax, QWORD PTR [rbp-16]
	sal	rax, 4
	add	rax, rbp
	sub	rax, 288
	mov	rax, QWORD PTR [rax]
	mov	esi, 0
	mov	rdi, rax
	call	pthread_join
	add	QWORD PTR [rbp-16], 1
.L8:
	cmp	QWORD PTR [rbp-16], 15
	jbe	.L9
	mov	eax, DWORD PTR [rbp-292]
	mov	DWORD PTR [rbp-20], eax
	mov	eax, DWORD PTR [rbp-20]
	mov	esi, eax
	mov	edi, OFFSET FLAT:.LC0
	mov	eax, 0
	call	printf
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (SUSE Linux) 14.2.0"
	.section	.note.GNU-stack,"",@progbits
