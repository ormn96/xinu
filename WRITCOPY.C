/* writcopy.c - writcopy */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  writcopy  --  high-speed copy from user's buffer into tty buffer
 *------------------------------------------------------------------------
 */
writcopy(buff, ttyp, avail, count)
register char *buff;
struct	tty *ttyp;
int	avail, count;
{
	register char	*qhead;
	int	ct, i;

	i = ttyp->ohead;
	qhead = &ttyp->obuff[i];
	for ( ct=count; ct>0; ct-- ) {
		*qhead++ = *buff++;
		if ( ++i >= OBUFLEN ) {	/* wrap-around			*/
			i=0;
			qhead = ttyp->obuff;
		}
	}
	ttyp->ocnt += count;
	ttyp->ohead = i;
	sreset(ttyp->osem, avail-count);
}
