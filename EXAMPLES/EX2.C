/* ex2.c - xmain, prA, prB */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  xmain  --  example of creating processes in PC-Xinu
 *------------------------------------------------------------------------
 */
xmain()
{
	int prA(), prB();

	resume( create(prA, INITSTK, INITPRIO, "proc 1", 0) );
	resume( create(prB, INITSTK, INITPRIO, "proc 2", 0) );
}

/*------------------------------------------------------------------------
 *  prA  --  repeatedly print 'A' without ever terminating
 *------------------------------------------------------------------------
 */
prA()
{
	while (1)
		putc(CONSOLE, 'A');
}

/*------------------------------------------------------------------------
 *  prB  --  repeatedly print 'B' without ever terminating
 *------------------------------------------------------------------------
 */
prB()
{
	while (1)
		putc(CONSOLE, 'B');
}
