/* readcopy.c - readcopy */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  readcopy  --  high speed copy from tty buffer into user's buffer
 *------------------------------------------------------------------------
 */
readcopy(buff,ttyp,avail,count)
register char *buff;
struct tty *ttyp;
int avail,count;
{
	register char *qtail;		/* copy variable		*/
	int	ct, i;

	i = ttyp->itail;
	qtail = &ttyp->ibuff[i];	/* address of tail		*/
	for ( ct=count; ct>0; ct-- ) {
		*buff++ = *qtail++;
		if ( ++i >= IBUFLEN ) {	/* wrap-around			*/
			i=0;
			qtail = ttyp->ibuff;
		}
	}
	ttyp->itail = i;
	ttyp->icnt -= count;
	sreset(ttyp->isem,avail-count);
}
