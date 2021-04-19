/* sleep.c - sleep */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>

/*------------------------------------------------------------------------
 *  sleep  --  delay the calling process n seconds
 *------------------------------------------------------------------------
 */
SYSCALL	sleep(n)
int n;
{
	int	ps;
	
	if ( n<0 )
		return(SYSERR);
	if (n == 0) {
		disable(ps);
		resched();
		restore(ps);
		return(OK);
	}
	while (n >= TICSD) {
		sleept(TICSN);
		n -= TICSD;
	}
	if (n > 0)
		sleept( (int)( ((long)n*(long)TICSN) / (long)TICSD ) );
	return(OK);
}
