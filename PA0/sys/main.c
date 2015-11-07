/* user.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

void halt();
extern long zfunction( long param );
/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */

int proc(char c) {

    kprintf("%c", c);

    return 0;

}

int main()
{
	kprintf("\n\nHello World, Xinu lives\n\n");
	kprintf( "Call zfunction, result is 0x%08x\n", zfunction(0xaabbccdd) );
	printsegaddress();
	printprocstks( -1 );
	printtos();
	syscallsummary_start();
	getpid();
	getpid();
	int prA, prB, prC;
	resume(prA = create(proc, 2000, 5, "proc A", 1, 'A'));
	resume(prB = create(proc, 2000, 50, "proc B", 1, 'B'));
	resume(prC = create(proc, 2000, 90, "proc C", 1, 'C'));
	printsyscallsfreq();
	syscallsummary_stop();

	return 0;
}
