/* mfclose.c - mfclose */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mffile.h>

/*------------------------------------------------------------------------
 *  mfclose  --  close a file by flushing output and freeing device slot
 *------------------------------------------------------------------------
 */
mfclose(devptr)
struct	devsw	*devptr;
{
	struct	mfblk	*mfptr;
	int	diskdev;
	int	ps;

	disable(ps);
	mfptr = (struct mfblk *) devptr->dvioblk;
	if (mfptr->mf_pid != currpid) {
		restore(ps);
		return(SYSERR);
	}
	if ( mfptr->mf_mode & FLWRITE )
		mfsflush(mfptr);
	dos_close(mfptr->mf_handle);
	mfptr->mf_pid = 0;
	restore(ps);
	return(OK);
}
