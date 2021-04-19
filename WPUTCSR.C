/* wputcsr.c - wputcsr */

#include <conf.h>
#include <kernel.h>
#include <tty.h>

/*------------------------------------------------------------------------
 *  wputcsr  --  cursor position routine
 *------------------------------------------------------------------------
 */

wputcsr(iptr,csr)
struct	tty	*iptr;
CURSOR csr;
{
	csr.row += iptr->topleft.row;
	csr.col += iptr->topleft.col;
	putcsrpos(csr,0);
}
