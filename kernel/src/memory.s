	.file	"memory.c"
	.text
	.align 16
	.globl	memcpy
	.type	memcpy, @function
memcpy:
.LFB0:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	24(%esp), %ebx
	movl	16(%esp), %esi
	movl	20(%esp), %edi
	testl	%ebx, %ebx
	jne	.L12
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L12:
	.cfi_restore_state
	call	get_cpu_info
	testb	$2, 27(%eax)
	movl	%ebx, 24(%esp)
	movl	%edi, 20(%esp)
	movl	%esi, 16(%esp)
	je	.L3
	cmpl	$127, %ebx
	ja	.L13
.L3:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	jmp	memcpy_normal
	.align 16
.L13:
	.cfi_restore_state
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	jmp	memcpy_sse
	.cfi_endproc
.LFE0:
	.size	memcpy, .-memcpy
	.align 16
	.globl	memcmp
	.type	memcmp, @function
memcmp:
.LFB1:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	movl	20(%esp), %ecx
	movl	12(%esp), %esi
	movl	16(%esp), %edx
	testl	%ecx, %ecx
	je	.L18
	movzbl	(%edx), %eax
	cmpb	%al, (%esi)
	jne	.L20
	leal	1(%esi), %eax
	addl	$1, %edx
	addl	%ecx, %esi
	jmp	.L16
	.align 16
.L17:
	movzbl	(%eax), %ebx
	movzbl	(%edx), %ecx
	addl	$1, %eax
	addl	$1, %edx
	cmpb	%cl, %bl
	jne	.L20
.L16:
	cmpl	%esi, %eax
	jne	.L17
.L18:
	movl	$1, %eax
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L20:
	.cfi_restore_state
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE1:
	.size	memcmp, .-memcmp
	.ident	"GCC: (GNU) 8.2.0"
