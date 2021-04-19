/* mkpool.c - mkpool */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <mem.h>
#include <bufpool.h>

/*------------------------------------------------------------------------
 *  mkpool  --  allocates buffers for a buffer pool
 *              and links them together
 *------------------------------------------------------------------------
 */

int mkpool(bufsiz, numbufs)
int	bufsiz, numbufs;
{
	int	ps;
	int	poolid;
	char	*where;


	disable(ps);
#ifdef MEMMARK
	if ( unmarked(bpmark) )
		poolinit();
#endif
	bufsiz = (bufsiz + 1) & ~1;	/* round up		*/
	if ( bufsiz<BPMINB ||
	     bufsiz>BPMAXB ||
	     numbufs<1 ||
	     numbufs>BPMAXN ||
	     nbpools >= NBPOOLS ||
	     (where=getmem((bufsiz+sizeof(int))*numbufs)) == NULL ) {
		restore(ps);
		return(SYSERR);
	}
	poolid = nbpools++;
	bptab[poolid].bpnext = (int *)where;
	bptab[poolid].bpsize = bufsiz;
	bptab[poolid].bpsem  = screate(numbufs);
	bufsiz += sizeof(int);
	for ( numbufs-- ; numbufs>0 ; numbufs--, where += bufsiz )
		*( (int *) where) = bufsiz+(int)where;
	*( (int *) where) = (int) NULL;
	restore(ps);
	return(poolid);
}
