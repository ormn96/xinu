/* printf.c - printf */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  printf  --  write formatted output to CONSOLE
 *------------------------------------------------------------------------
 */
printf(fmt,args)
char *fmt;
int args;
{
	int	putc();
	
	_doprnt(fmt, &args, putc, CONSOLE);
	return(OK);
}
