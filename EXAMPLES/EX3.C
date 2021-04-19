/* ex3.c - xmain, prntr */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  xmain  --  example of 2 processes executing the same code concurrently
 *------------------------------------------------------------------------
 */
xmain()
{
	int prntr();

	resume( create(prntr, INITSTK, INITPRIO, "print A", 1, 'A') );
	resume( create(prntr, INITSTK, INITPRIO, "print B", 1, 'B') );
}
	
/*------------------------------------------------------------------------
 *  prntr  --  print a character indefinitely
 *------------------------------------------------------------------------
 */
prntr( ch )
int ch;
{
	while (1)
		putc(CONSOLE, ch);
}
