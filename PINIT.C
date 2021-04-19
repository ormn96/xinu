/* pinit.c - pinit */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <mark.h>
#include <ports.h>

#ifdef	MEMMARK
MARKER	ptmark;
#endif

struct	ptnode 	*ptfree;	/* list of free queued nodes */
struct	pt	ports[NPORTS];
int		ptnextp;

/*------------------------------------------------------------------------
 *  pinit  --  initialize all of the ports
 *------------------------------------------------------------------------
 */

SYSCALL pinit()
{
	int		i;
	struct	ptnode	*next, *prev;
	int maxmsgs;
#ifdef	MEMMARK
	int status;

	if ( (status=mark(ptmark)) == SYSERR )
		panic("pinit - memory marking error");
	if ( status == 0 )
		return(OK);		/* already marked		*/
#endif
	maxmsgs = MAXMSGS;
	if ( (ptfree=(struct ptnode *)getmem(maxmsgs*sizeof(struct ptnode)))
		== (struct ptnode *)NULL ) {
#ifdef MEMMARK
		panic("pinit - insufficient memory");
#else
		return(SYSERR);
#endif
	}
	for (i=0 ; i<NPORTS ; i++)
		ports[i].ptstate = PTFREE;
	ptnextp = NPORTS - 1;

	/*  link up free list of message pointer nodes  */

	for ( prev=next=ptfree ; --maxmsgs > 0 ; prev=next )
		prev->ptnext = ++next;
	prev->ptnext = (struct ptnode *)NULL;
	return(OK);
}
