/* mfputc.c - mfputc */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mffile.h>

/*------------------------------------------------------------------------
 *  mfputc  --  put a character onto a (buffered) disk file
 *------------------------------------------------------------------------
 */
mfputc(devptr, ch)
struct	devsw	*devptr;
char	ch;
{
	struct	mfblk	*mfptr;
	int	ps;

	disable(ps);
	mfptr = (struct mfblk *) devptr->dvioblk;
	if (mfptr->mf_pid != currpid || !(mfptr->mf_mode&FLWRITE)) {
		restore(ps);
		return(SYSERR);
	}
	if ( mfptr->mf_bptr >= &mfptr->mf_buff[DBUFSIZ] ) {
		mfsflush(mfptr);
		mfsetup(mfptr);
	}
	mfptr->mf_pos++;
	if ( mfptr->mf_pos > mfptr->mf_size )
		mfptr->mf_size = mfptr->mf_pos;
	*(mfptr->mf_bptr)++ = ch;
	mfptr->mf_dch = TRUE;
	restore(ps);
	return(OK);
}
