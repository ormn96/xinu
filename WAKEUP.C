/* wakeup.c - wakeup */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>

/*------------------------------------------------------------------------
 *  wakeup  --  called by clock interrupt dispatcher to awaken processes
 *------------------------------------------------------------------------
 */

void wakeup()
{
        long temp;

        while ( nonempty(clockq) && (firstkey(clockq) <= 0) ) 
                ready(getfirst(clockq));
                
        if ( (slnempty = nonempty(clockq)) != 0 ) 
		sltop = &firstkey(clockq);

} /* wakeup */
