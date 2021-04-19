/* mfsetup.c - mfsetup */

#include <conf.h>
#include <kernel.h>
#include <mffile.h>

/*------------------------------------------------------------------------
 *  mfsetup  --  set up appropriate data block in memory
 *------------------------------------------------------------------------
 */
mfsetup(mfptr)
struct	mfblk	*mfptr;
{
	int	offset;
	long	dos_lseek();

	mfptr->mf_bpos = (mfptr->mf_pos)&DBMASK;
	offset = mfptr->mf_pos - mfptr->mf_bpos;
	if ( mfptr->mf_bpos < mfptr->mf_size ) {
		dos_lseek(mfptr->mf_handle,mfptr->mf_bpos,0);
		dos_read(mfptr->mf_handle,mfptr->mf_buff,DBUFSIZ);
	}
	mfptr->mf_bptr = mfptr->mf_buff + offset;
	mfptr->mf_dch = FALSE;
}
