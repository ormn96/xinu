/* ttyputc.c - ttyputc */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  ttyputc  --  write one character to a tty device
 *------------------------------------------------------------------------
 */
ttyputc(devptr, ch )
struct	devsw	*devptr;
char	ch;
{
	struct	tty   *iptr;
	int	ps;

	iptr = &tty[devptr->dvminor];
	disable(ps);
	wait(iptr->osem);		/* wait	for space in queue	*/
	iptr->obuff[iptr->ohead++] = ch;
	++iptr->ocnt;
	if (iptr->ohead	>= OBUFLEN)
		iptr->ohead = 0;
	restore(ps);
	sendn(iptr->oprocnum,TMSGOK);	/* wake up the tty process	*/
	return(OK);
}
