#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>


SYSCALL xforkSonFirst(){
	int newPid;
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */
	
	newPid = xfork();
	if(newPid == SYSERR)
		return SYSERR;
	
	if(newPid !=0 ){//only father

		//from resched
		optr = &proctab[currpid];
		/* force context switch */
		ready(currpid);
		
		nptr = &proctab[ (currpid = dequeue(newPid)) ];
		nptr->pstate = PRCURR;		/* mark it currently running	*/
		preempt = QUANTUM;		/* reset preemption counter	*/
		ctxsw(&optr->pregs,&nptr->pregs);
		//from resched
	}
	return newPid;
}