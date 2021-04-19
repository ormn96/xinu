/* mfinit.c - mfinit */

#include <conf.h>
#include <kernel.h>
#include <mffile.h>

/*------------------------------------------------------------------------
 *  mfinit  --  mark disk file 'device' available at system startup
 *------------------------------------------------------------------------
 */
mfinit(devptr)
struct	devsw	*devptr;
{
	struct	mfblk	*mfptr;

	devptr->dvioblk = (char *)(mfptr = &mftab[devptr->dvminor]);
	mfptr->mf_pid = 0;
	mfptr->mf_id = devptr->dvnum;
	return(OK);
}
