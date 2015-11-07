	.file	"printtos.c"
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align 4
.LC0:
	.string	"The content of %d stack locations below the top of stack is 0x%08x.\n"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"%d\n"
	.text
.globl printtos
	.type	printtos, @function
printtos:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$4, %esp
#APP
	movl %esp, esp
#NO_APP
	movl	$-50, %ebx
.L5:
	subl	$4, %esp
	movl	esp, %eax
	pushl	(%eax,%ebx,4)
	pushl	%ebx
	pushl	$.LC0
	call	kprintf
	addl	$16, %esp
	incl	%ebx
	cmpl	$49, %ebx
	jle	.L5
	subl	$8, %esp
	pushl	$200
	pushl	$.LC1
	call	kprintf
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	printtos, .-printtos
	.local	esp
	.comm	esp,4,4
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.4.6 20060404 (Red Hat 3.4.6-3)"
