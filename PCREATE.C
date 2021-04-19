/* pcreate.c - pcreate */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <ports.h>

/*------------------------------------------------------------------------
 *  pcreate  --  create a port that allows "count" outstanding messages
 *------------------------------------------------------------------------
 */

SYSCALL pcreate(count)
int count;
{
	int	ps;
	int	i, p;
	struct	pt	*ptptr;
	int	mkval;

	if ( count < 0 || count > MAXMSGS )
		return(SYSERR);
	disable(ps);
#ifdef	MEMMARK
	if ( unmarked(ptmark) )
		pinit();
#endif
	for (i=0 ; i<NPORTS ; i++) {
		if ( (p=ptnextp--) <= 0 )
			ptnextp = NPORTS - 1;
		if ( (ptptr= &ports[p])->ptstate == PTFREE ) {
			ptptr->ptstate = PTALLOC;
			ptptr->ptssem  = screate(count);
			ptptr->ptrsem  = screate(0);
			ptptr->pthead  = ptptr->pttail = (struct ptnode *)NULL;
			ptptr->ptseq++;
			ptptr->ptmaxcnt = count;
			ptptr->ptct = 0;
			restore(ps);
			return(p);
		}
	}
	restore(ps);
	return(SYSERR);
}
