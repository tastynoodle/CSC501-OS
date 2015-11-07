#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

static unsigned long *esp;
void printprocstks( int priority )
{
	kprintf( "Call function printprocstks.\n" );
	
	int i;
	struct pentry *proc;
	for( i = 0; i < NPROC; i++ )
	{
		proc = &proctab[i];

		if( ( proc->pstate != PRFREE ) && ( proc->pprio > priority ) )
		{
			kprintf( "Process Name: %s\n", proc->pname );
			kprintf( "Process id: %d\n", i );
			kprintf( "Process priority: %d\n", proc->pprio );
			kprintf( "Stack base: 0x%08x\n", proc->pbase );
			kprintf( "Stack size: %d\n", proc->pstklen );
			kprintf( "Stack limit: 0x%08x\n", proc->plimit );

			if( i == currpid )
			{
				asm( "movl %esp, esp" );
				kprintf( "Stack pointer: 0x%08x\n\n", esp );
			}
			else
			{
				kprintf( "Stack pointer: 0x%08x\n\n", proc->pesp );
			}
		}
	}
}
