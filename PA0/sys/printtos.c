#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

static unsigned long *esp;
void printtos()
{
	kprintf( "Call function printtos.\n" );

	asm( "movl %esp, esp" );
	kprintf( "Before get into printtos function call, the top of stack is 0x%08x.\n", esp+2 );
	kprintf( "After get into printtos function call, the top of stack is 0x%08x.\n", esp );

	int i = 0;
	for ( i = 1; i < 7; i++ )
	{
		kprintf( "The content of %d stack locations below the top of stack is 0x%08x.\n", i, *(esp+i) );
	}
	kprintf( "\n" );
}
