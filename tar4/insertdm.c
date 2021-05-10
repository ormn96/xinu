#include "tmpmem.h"


void tmpMemInsertd(id, key)
	int	id;
	int	key;
{
	int	next;			/* runs through list		*/
	int	prev;			/* follows next through list	*/

	for(prev=tmpMemHead,next=qmem[tmpMemHead].qnext ;
	    qmem[next].qkey < key ; prev=next,next=qmem[next].qnext)
		key -= qmem[next].qkey;
	qmem[id].qnext = next;
	qmem[id].qprev = prev;
	qmem[id].qkey  = key;
	qmem[prev].qnext = id;
	qmem[next].qprev = id;
	if (next < NMEM)
		qmem[next].qkey -= key;
}
