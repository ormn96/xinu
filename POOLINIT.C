/* poolinit.c - poolinit */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <bufpool.h>

struct bpool bptab[NBPOOLS];
int    nbpools;

#ifdef MEMMARK
MARKER bpmark;
#endif

/*------------------------------------------------------------------------
 *  poolinit  --  initialize the buffer pool
 *------------------------------------------------------------------------
 */

SYSCALL poolinit()
{
#ifdef 	MEMMARK
	int	status;

	if ( (status=mark(bpmark)) == SYSERR )
		panic("poolinit - memory marking error");
	if ( status == 0 )		/* already marked		*/
		return(OK);
#endif
	nbpools = 0;
	return(OK);
}
