/* open.c - open */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  open  --  open a connection to a device/file (arg1 & arg2 optional)
 *------------------------------------------------------------------------
 */
open(descrp, arg1, arg2)
int	descrp;
char	*arg1, *arg2;
{
	struct	devsw	*devptr;

	if ( isbaddev(descrp) )
		return(SYSERR);
	devptr = &devtab[descrp];
	return(	(*devptr->dvopen)(devptr, arg1, arg2) );
}
