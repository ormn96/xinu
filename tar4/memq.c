#include <q.h>
#include "tmpmem.h"

int	tmpMemGetfirst()
{
	int	head;			/* first mem on the list	*/

	if ((head=qmem[tmpMemHead].qnext) < NMEM)
		return( tmpMemDequeue(head) );
	else
		return(EMPTY);
}

int tmpMemDequeue(item)
int item;
{
	struct	qentmem	*mptr;		/* pointer to q entry for item	*/

	mptr = &qmem[item];
	qmem[mptr->qprev].qnext = mptr->qnext;
	qmem[mptr->qnext].qprev = mptr->qprev;
	return(item);
}
