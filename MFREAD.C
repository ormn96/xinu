/* mfread.c - mfread */

#include <conf.h>
#include <kernel.h>
#include <mffile.h>

/*------------------------------------------------------------------------
 *  mfread  --  read from a previously opened disk file
 *------------------------------------------------------------------------
 */
mfread(devptr, buff, count)
struct	devsw	*devptr;
char	*buff;
int	count;
{
	int	done;
	int	ichar;

	if (count < 0)
		return(SYSERR);
	for (done=0 ; done < count ; done++)
		if ( (ichar=mfgetc(devptr)) == SYSERR)
			return(SYSERR);
		else if (ichar == EOF ) {	/* EOF before finished */
			if (done == 0)
				return(EOF);
			else
				return(done);
		} else
			*buff++ = (char) ichar;
	return(done);
}
