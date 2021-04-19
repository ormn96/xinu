/* scount.c - scount */

#include <conf.h>
#include <kernel.h>
#include <sem.h>

/*------------------------------------------------------------------------
 *  scount  --  return a semaphore count
 *------------------------------------------------------------------------
 */
SYSCALL scount(sem)
int sem;
{
	extern	struct	sentry	semaph[];
	int	ps;
	int	ct;

	disable(ps);
	if (isbadsem(sem) || semaph[sem].sstate==SFREE) {
		restore(ps);
		return(SYSERR);
	}
	ct = semaph[sem].semcnt;
	restore(ps);
	return(ct);
}
