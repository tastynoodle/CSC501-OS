/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sched.h>

unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);
/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
int endof_epoch( int ptype )
{
	int i;
	struct pentry *p;
	for( i = 0; i < NPROC; i++ )
        {
                p = &proctab[i];
                if( (p->ptype == ptype) && (p->pstate == PRREADY || p->pstate == PRCURR) && (p->counter > 0) )
			return 0;
        }
	return 1;
}

int new_epoch( int ptype )
{
	int i;
	struct pentry *p;

	for( i = 0; i < NPROC; i++ )
	{
		p = &proctab[i];
		if( p->pstate == PRFREE || p->ptype != ptype ) continue;

		if( ptype == NORMAL )
		{
			if( p->counter <= 0 || p->counter == p->quantum )
				p->quantum = p->pprio;
			else
				p->quantum = p->counter/2 + p->pprio;

			p->counter = p->quantum;
			p->goodness = p->counter + p->pprio;
		}
		else
		{
			p->counter = p->quantum;
		}
	}
	return OK;
}

// get the id of process with the largest goodness
int get_next_pid( int ptype )
{
	int i = 0;
	struct pentry *p;
	int res = 0;

	if( ptype == NORMAL )
	{
		int largest_goodness = 0;
		for( i = q[rdyhead].qnext; i != rdytail; i = q[i].qnext )
        	{
			p = &proctab[i];
        		if( (p->ptype == ptype) && (p->goodness > largest_goodness) )
			{
				res = i;
				largest_goodness = p->goodness;
			}
        	}
	}
	else
	{
		int largest_pprio = 0;
		for( i = q[rdyhead].qnext; i != rdytail; i = q[i].qnext )
		{
			p = &proctab[i];
			if( (p->ptype == ptype) && (p->pprio > largest_pprio) )
			{
				res = i;
				largest_pprio = p->pprio;
			}
		}
	}
	return res;
}

int xinu_sched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

	/* no switch needed if current process priority higher than next*/

	if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
	   (lastkey(rdytail)<optr->pprio)) {
		return(OK);
	}
	
	/* force context switch */

	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}

	/* remove highest priority process at end of ready list */

	nptr = &proctab[ (currpid = getlast(rdytail)) ];
	nptr->pstate = PRCURR;		/* mark it currently running	*/
#ifdef	RTCLOCK
	preempt = QUANTUM;		/* reset preemption counter	*/
#endif
	
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
	/* The OLD process returns here when resumed. */
	return OK;
}

int linux_sched2()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

	optr = &proctab[currpid];

	int nextpid = get_next_pid( NORMAL );

	/* no switch needed */
	if( optr->ptype == NORMAL && optr->goodness >= proctab[nextpid].goodness && optr->pstate == PRCURR && optr->counter > 0 )
	{
		if( optr->counter < QUANTUM ) 
			preempt = optr->counter;
		else
			preempt = QUANTUM;
		return OK;
	}

	/* force context switch */
	if (optr->pstate == PRCURR)
	{
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}
	nptr = &proctab[nextpid];
	nptr->pstate = PRCURR;		/* mark it currently running	*/
	dequeue(nextpid);
	currpid = nextpid;
	if( nptr->counter < QUANTUM )
		preempt = nptr->counter;
	else
		preempt = QUANTUM;
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	return OK;
}

int real_sched2()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

	optr = &proctab[currpid];

	int nextpid = get_next_pid( REAL_TIME );

	/* no switch needed */
	if( (optr->ptype == REAL_TIME) && (optr->pprio >= proctab[nextpid].pprio) && (optr->pstate == PRCURR) && (optr->counter > 0) )
	{
		if( optr->counter < QUANTUM ) 
			preempt = optr->counter;
		else
			preempt = QUANTUM;
		return OK;
	}

	/* force context switch */
	if (optr->pstate == PRCURR)
	{
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}
	nptr = &proctab[nextpid];
	nptr->pstate = PRCURR;		/* mark it currently running	*/
	dequeue(nextpid);
	currpid = nextpid;
	if( nptr->counter < QUANTUM )
		preempt = nptr->counter;
	else
		preempt = QUANTUM;
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	return OK;
}

int currq = -1;
int linux_sched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */

	optr = &proctab[currpid];

	optr->counter -= (QUANTUM - preempt);
	if (optr->counter <= 0)
	{
		optr->counter = 0;
		optr->goodness = 0;
	}
	else
	{
		optr->goodness = optr->counter + optr->pprio;
	}

	if ( endof_epoch( NORMAL ) )
	{
		int rand_num = rand()%10;
		if( (rand_num < 7 && !isqempty(REAL_TIME)) || (rand_num >=7 && isqempty(NORMAL)))
		{	
			new_epoch( REAL_TIME );
			currq = REAL_TIME;
			return real_sched2();
		}
		else
		{
			new_epoch( NORMAL );
			currq = NORMAL;
			return linux_sched2();
		}
	}

	return linux_sched2();
}

// run the real-time processes
int real_sched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */

	optr = &proctab[currpid];

	optr->counter -= (QUANTUM - preempt);
	if (optr->counter <= 0)
		optr->counter = 0;

	if ( endof_epoch( REAL_TIME ) )
	{
		int rand_num = rand()%10;
		if( (rand_num < 7 && !isqempty(REAL_TIME)) || (rand_num >=7 && isqempty(NORMAL)))
		{	
			new_epoch( REAL_TIME );
			currq = REAL_TIME;
			return real_sched2();
		}
		else
		{
			new_epoch( NORMAL );
			currq = NORMAL;
			return linux_sched2();
		}
	}

	return real_sched2();
}

int isqempty( int ptype )
{
        int i;
        struct pentry *p;
        for( i = 0; i < NPROC; i++ )
        {
                p = &proctab[i];
                if( (p->ptype == ptype) && (p->pstate == PRREADY || p->pstate == PRCURR) )
                        return 0;
        }
        return 1;
}

int multiq_sched()
{
	int rand_num;
	switch(currq)
	{
		case -1:
			rand_num = rand()%10;
			if( (rand_num < 7 && !isqempty(REAL_TIME)) || (rand_num >=7 && isqempty(NORMAL))) 
			{
				currq = REAL_TIME;
				return real_sched();
			}
			else
			{
				currq = NORMAL;
				return linux_sched();
			}
			break;
		case NORMAL:
			return linux_sched();
		case REAL_TIME:
			return real_sched();
		default:
			break;
	}

}

int resched()
{
        switch( getschedclass() )
        {
                case XINUSCHED:
                        return xinu_sched();
                case LINUXSCHED:
                        return linux_sched();
                case MULTIQSCHED:
               		return multiq_sched();
                default:
                        break;
        }
	return OK;
}
