/* mfsflush.c - mfsflush */

#include <conf.h>
#include <kernel.h>
#include <mffile.h>

/*------------------------------------------------------------------------
 *  mfsflush  --  flush data block for an MS-DOS file
 *------------------------------------------------------------------------
 */
mfsflush(mfptr)
struct	mfblk	*mfptr;
{
	int	bufcnt;
	long	dos_lseek();

	if ( mfptr->mf_dch == FALSE )
		return;
	bufcnt = DBUFSIZ;
	if (mfptr->mf_pos < mfptr->mf_bpos+DBUFSIZ)
		bufcnt = mfptr->mf_pos - mfptr->mf_bpos;
	dos_lseek(mfptr->mf_handle,mfptr->mf_bpos,0 );
	dos_write(mfptr->mf_handle,mfptr->mf_buff,bufcnt);
	mfptr->mf_dch = FALSE;
}
