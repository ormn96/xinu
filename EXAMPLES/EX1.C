/* ex1.c - xmain */

#include <conf.h>

/*------------------------------------------------------------------------
 *  xmain  --  write "hi" on the CONSOLE
 *------------------------------------------------------------------------
 */
xmain()
{
	putc(CONSOLE,'h');
	putc(CONSOLE,'i');
	putc(CONSOLE,'\n');
}
