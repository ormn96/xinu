/* ttygetc.c - ttygetc */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  ttygetc  --  read one character from a tty device
 *------------------------------------------------------------------------
 */
ttygetc(devptr)
struct	devsw	*devptr;
{
	int	ps;
	char	ch;
	struct	tty   *iptr;

	disable(ps);
	iptr = &tty[devptr->dvminor];
	wait(iptr->isem);		/* wait	for a character	in buff	*/
	ch = iptr->ibuff[iptr->itail++];
	--iptr->icnt;
	if (iptr->itail	>= IBUFLEN)
		iptr->itail = 0;
	restore(ps);
	return(ch);
}
