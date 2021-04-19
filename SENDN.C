/* sendn.c - sendn */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  sendn  --  send a message to another process, but do not reschedule
 *------------------------------------------------------------------------
 */
SYSCALL	sendn(pid, msg)
int	pid;
int	msg;
{
	struct	pentry	*pptr;		/* receiver's proc. table addr.	*/
	int	ps;

	disable(ps);
	if (isbadpid(pid) || ( (pptr= &proctab[pid])->pstate == PRFREE)
	   || pptr->phasmsg != 0) {
		restore(ps);
		return(SYSERR);
	}
	pptr->pmsg = msg;		/* deposit message		*/
	pptr->phasmsg++;
	if (pptr->pstate == PRRECV)	/* if receiver waits, start it	*/
		ready(pid);
	restore(ps);
	return(OK);
}
