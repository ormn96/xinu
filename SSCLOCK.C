/* ssclock.c - stopclk, strtclk */

#include <conf.h>
#include <kernel.h>
#include <q.h>
#include <sleep.h>

/*------------------------------------------------------------------------
 *  stopclk  --  put the clock in defer mode
 *------------------------------------------------------------------------
 */
stopclk()
{
	int	ps;

	disable(ps);
	defclk++;
	restore(ps);
}

/*------------------------------------------------------------------------
 *  strtclk  --  take the clock out of defer mode
 *------------------------------------------------------------------------
 */
strtclk()
{
	int	ps;
	int makeup;
	int next;

	disable(ps);
	if ( defclk<=0 || --defclk>0 ) {
		restore(ps);
		return;
	}
	makeup = clkdiff;
	clkdiff = 0;
	if (slnempty)
		if ( (*sltop -= makeup) <= 0 )
			wakeup();
	if ( (preempt -= makeup) <= 0 )
		resched();
	restore(ps);
}
