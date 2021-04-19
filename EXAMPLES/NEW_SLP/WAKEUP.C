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

int sleep_time[NPROC];

void new_wakeup()
{
        int pid;
        long temp;

        while ( nonempty(clockq) && (firstkey(clockq)) <= 0 ) {
                pid = getfirst(clockq);
                
                if (sleep_time[pid] == 0)
                  ready(pid);
                else
                  if ( sleep_time[pid] > TICSD )
                     {
                      insertd(pid,clockq, TICSN);
                      sleep_time[pid] -= TICSD;
                     } /* if */
                  else
                  {
                      temp =  (( (long)sleep_time[pid] )*TICSN)/TICSD;  
                      insertd(pid,clockq, temp);
                      sleep_time[pid] = 0;
                  } /* else */
	}
        if ( (slnempty = nonempty(clockq)) != 0 ) 
		sltop = &firstkey(clockq);
}
