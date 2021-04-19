/* pcount.c - pcount */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <ports.h>

/*------------------------------------------------------------------------
 *  pcount  --  return the count of current messages in a port
 *------------------------------------------------------------------------
 */
SYSCALL	pcount(portid)
int	portid;
{
	int	scnt;
	int	count;
	int	ps;
	struct	pt	*ptptr;

	disable(ps);
	if ( isbadport(portid) ||
#ifdef MEMMARK
		unmarked(ptmark) ||
#endif
		(ptptr= &ports[portid])->ptstate != PTALLOC ) {
			restore(ps);
			return(SYSERR);
	}
	count = ptptr->ptct;
	restore(ps);
	return(count);
}
