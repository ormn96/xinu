/* sleept.c - sleept */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>

/*------------------------------------------------------------------------
 *  sleept  --  delay the caller for a time specified in system ticks
 *------------------------------------------------------------------------
 */
SYSCALL	sleept(n)
int n;
{
	int	ps;

	if ( n<0 )
		return(SYSERR);
	if (n == 0)
		return(OK);
        disable(ps);
        insertd(currpid,clockq,n);
	slnempty = TRUE;
	sltop = & q[q[clockq].qnext].qkey;
        proctab[currpid].pstate = PRSLEEP;
	resched();
        restore(ps);
	return(OK);
}
