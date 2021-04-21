/* clkint.c - clkint */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <io.h>
#include <q.h>//CHANGE
#include <proc.h>//CHANGE
#include "new.h"//CHANGE


/*------------------------------------------------------------------------
 *  clkint  --  clock service routine
 *  called at every clock tick and when starting the deferred clock
 *------------------------------------------------------------------------
 */
INTPROC clkint(mdevno)
int mdevno;				/* minor device number		*/
{
	int	i;
        int resched_flag;
		int cur;

        
	tod++;

	
	    resched_flag = 0;
	//CHANGE
	
	current_time[currpid]++;
	peffec[currpid] = calc_peffec(currpid);
	
	//run on ready list
	cur = q[rdyhead].qnext;
	while (cur!= rdytail){
		runnable_time[cur]++;
		peffec[cur] = calc_peffec(cur);
		cur = q[cur].qnext;
	}
	fixReady();
	if(peffec[currpid]< lastkeyeffec(rdytail))
		resched_flag = 1;
	
	//END_CHANGE
	
	if (slnempty)
		if ( (--*sltop) <= 0 )
                     {
                        resched_flag = 1;
			wakeup();
                     } /* if */
	if ( (--preempt) <= 0 )
             resched_flag = 1;

       if (resched_flag == 1)
 		resched();

}

