/* mfdsrch.c - mfdsrch */

#include <conf.h>
#include <kernel.h>
#include <mffile.h>

#define MSREAD	0
#define MSWRITE	1
#define MSRDWR	2

/*------------------------------------------------------------------------
 *  mfdsrch  --  search dos for given file name
 *------------------------------------------------------------------------
 */
mfdsrch(filenam, mbits)
char	*filenam;
int	mbits;
{
	int	fd;		/* MS-DOS file handle */
	int	mode;		/* MS-DOS mode bits */

	if ( strlen(filenam) <= 0 )
		return(SYSERR);
	mode = (mbits&FLRW) - 1;
	if ( mode<0 || mode>MSRDWR )
		return(SYSERR);
	if ( (fd=dos_open(filenam,mode)) == SYSERR ) {
		if ( mbits&FLOLD )
			return(SYSERR);
		else 	/* create a new file with no special attributes */
			return( dos_creat(filenam,0) );
	}
	if ( mbits&FLNEW ) {
		dos_close(fd);
		return(SYSERR);
	}
	return(fd);
}
