/* msopen.c - msopen */

#include <conf.h>
#include <kernel.h>
#include <mffile.h>

/*------------------------------------------------------------------------
 *  msopen  --  open/create a dos file
 *------------------------------------------------------------------------
 */
msopen(devptr,filenam,mode)
struct devsw *devptr;
char *filenam;
char *mode;
{
	struct mfblk	*mfptr;
	int	mbits;
	int	findex;
	int	ps;
	int	fd;		/* DOS file handle */
	long	size;		/* file size */
	long	dos_lseek();

	if ( (mbits=dfckmd(mode)) == SYSERR )
		return(SYSERR);
	disable(ps);
	if ( (fd=mfdsrch(filenam,mbits)) == SYSERR ) {
		restore(ps);
		return(SYSERR);
	}
	/* determine file size */
	if ( (size=dos_lseek(fd,0L,2)) == (long)SYSERR ) {
		dos_close(fd);
		restore(ps);
		return(SYSERR);
	}
	if ( (findex=mfalloc()) == SYSERR ) {
		dos_close(fd);
		restore(ps);
		return(SYSERR);
	}
	mfptr = &mftab[findex];
	mfptr->mf_dev = devptr->dvnum;
	mfptr->mf_handle = fd;
	mfptr->mf_mode = mbits & (FLRW | FLBINARY);
	mfptr->mf_size = size;
	mfptr->mf_pos = 0L;
	mfptr->mf_bpos = 0L;
	mfptr->mf_dch = FALSE;
	if ( size > 0L) {
		dos_lseek(fd,0L,0);		/* reset to beginning */
		dos_read(fd,mfptr->mf_buff,DBUFSIZ);
		mfptr->mf_bptr = mfptr->mf_buff;
	} else
		mfptr->mf_bptr = &mfptr->mf_buff[DBUFSIZ];
	restore(ps);
	return(mfptr->mf_id);
}
