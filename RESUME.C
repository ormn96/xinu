/* resume.c - resume */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  resume  --  unsuspend a process, making it ready; return the priority
 *------------------------------------------------------------------------
 */

SYSCALL resume(pid)
int pid;
{
	int	ps;			/* saved processor status	*/
	struct	pentry	*pptr;		/* pointer to proc. tab. entry	*/
	int	prio;			/* priority to return		*/

	disable(ps);
	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate != PRSUSP) {
		restore(ps);
		return(SYSERR);
	}
	prio = pptr->pprio;
	ready(pid);
	resched();
	restore(ps);
	return(prio);
}
