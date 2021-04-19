/* ttyread.c - ttyread */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  ttyread  --  read one or more characters from a tty device
 *------------------------------------------------------------------------
 */
ttyread(devptr, buff, count)
struct devsw *devptr;
char *buff;
int count;
{
	register struct	tty *ttyp;
	int	ps;
	int	avail, nread;

	if ( count<0 )
		return(SYSERR);
	disable(ps);
	ttyp = &tty[devptr->dvminor];
	avail = scount( ttyp->isem );
	if ( (count = (count==0 ? avail : count)) == 0) {
		restore(ps);
		return(0);
	}
	nread = count;
	if ( count <= avail )
		readcopy(buff, ttyp, avail, count);
	else {
		if (avail > 0) {
			readcopy(buff, ttyp, avail, avail);
			buff += avail;
			count -= avail;
		}
		for ( ; count>0 ; count--)
			*buff++ = ttygetc(devptr);
	}
	restore(ps);
	return(nread);
}
