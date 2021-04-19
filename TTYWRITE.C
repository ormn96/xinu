/* ttywrite.c - ttywrite */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  ttywrite  --  write one or more characters to a tty device
 *------------------------------------------------------------------------
 */
ttywrite(devptr, buff, count)
struct	devsw	*devptr;
char	*buff;
int	count;
{
	register struct tty *ttyp;
	int	avail;
	int	ps;

	if (count < 0)
		return(SYSERR);
	if (count == 0)
		return(0);
	disable(ps);
	ttyp = &tty[devptr->dvminor];
	avail = scount( ttyp->osem );
	if ( avail >= count) {
		writcopy(buff, ttyp, avail, count);
		sendn(ttyp->oprocnum,TMSGOK);
	} else {
		if (avail > 0) {
			writcopy(buff, ttyp, avail, avail);
			sendn(ttyp->oprocnum,TMSGOK);
			buff += avail;
			count -= avail;
		}
		for (; count>0 ; count--)
			ttyputc(devptr, *buff++);
	}
	restore(ps);
	return(count);
}
