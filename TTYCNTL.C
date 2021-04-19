/* ttycntl.c - ttycntl */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  ttycntl  --  control a tty device by setting modes
 *------------------------------------------------------------------------
 */
SYSCALL ttycntl(devptr, func)
struct devsw *devptr;
int func;
{
	register struct	tty *ttyp;
	int	ps;
	int	c;

	disable(ps);
	ttyp = &tty[devptr->dvminor];
	c = OK;				/* assume the best		*/
	switch ( func )	{
	case TCNEXTC:
		wait(ttyp->isem);
		c = ttyp->ibuff[ttyp->itail];
		signal(ttyp->isem);
		break;
	case TCMODER:
		ttyp->imode = IMRAW;
		break;
	case TCMODEC:
		ttyp->imode = IMCOOKED;
		break;
	case TCMODEK:
		ttyp->imode = IMCBREAK;
		break;
	case TCECHO:
		ttyp->iecho = TRUE;
		break;
	case TCNOECHO:
		ttyp->iecho = FALSE;
		break;
	case TCICHARS:
		c = scount(ttyp->isem);
		break;
	default:
		c = SYSERR;
	}
	restore(ps);
	return(c);
}
