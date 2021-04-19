/* ttyiin.c - ttyiin */

#include <conf.h>
#include <kernel.h>
#include <tty.h>

/*------------------------------------------------------------------------
 *  ttyiin  --  lower-half tty device driver for input interrupts
 *------------------------------------------------------------------------
 */
INTPROC	ttyiin()
{
	send(tty[0].wstate,TMSGOK);
}
