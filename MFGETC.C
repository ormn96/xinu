/* mfgetc.c - mfgetc */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mffile.h>

/*------------------------------------------------------------------------
 *  mfgetc  --  get next character from (buffered) disk file
 *------------------------------------------------------------------------
 */
mfgetc(devptr)
struct devsw *devptr;
{
	struct	mfblk	*mfptr;
	char	nextch;
	int	ps;

	disable(ps);
	mfptr = (struct mfblk *)devptr->dvioblk;
	if (mfptr->mf_pid!=currpid || !(mfptr->mf_mode&FLREAD)) {
		restore(ps);
		return(SYSERR);
	}
	if ( mfptr->mf_pos >= mfptr->mf_size ) {
		restore(ps);
		return(EOF);
	}
	if ( mfptr->mf_bptr >= &mfptr->mf_buff[DBUFSIZ] ) {
		mfsflush(mfptr);
		mfsetup(mfptr);
	}
	nextch = *(mfptr->mf_bptr)++;
	mfptr->mf_pos++;
	restore(ps);
	return( ((int)nextch) & 0xff );
}
