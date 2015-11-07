	.text
.globl zfunction
zfunction:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	andl	$0xfffff00f, %eax
	shll	$8, %eax
	leave
	ret
