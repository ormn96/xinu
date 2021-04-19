/* mfseek.c - mfseek */

#include <conf.h>
#include <kernel.h>
#include <mffile.h>

/*------------------------------------------------------------------------
 *  mfseek  --  seek to a specified position of a file
 *------------------------------------------------------------------------
 */
mfseek(devptr, offset)
struct	devsw	*devptr;
long	offset;
{
	struct	mfblk	*mfptr;
	int	retcode;
	int	ps;

	disable(ps);
	mfptr = (struct mfblk *)devptr->dvioblk;
	if ( mfptr->mf_mode & FLWRITE )
		mfsflush(mfptr);
	else if ( offset > mfptr->mf_size ) {
		restore(ps);
		return(SYSERR);
	}
	mfptr->mf_pos = offset;
	mfsetup(mfptr);
	restore(ps);
	return(OK);
}
