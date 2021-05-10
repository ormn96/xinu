/* kill.c - kill */
/* 8086 version */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <q.h>
#include <sleep.h>
#include "wait.h"//CHANGE

/*------------------------------------------------------------------------
 *  kill  --  kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
SYSCALL kill(pid)
	int	pid;		     /* process to kill		*/
{
	struct	pentry	*pptr;	     /* points to proc. table for pid */
	int	ps;	             /* saved processor status	*/
        int pstate;
	int f;//CHANGE father pid
	disable(ps);
	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate==PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	//CHANGE
		 if((f = father[pid])!=-1){
			 if(wait_cnt[f]!=-1){
				 wait_cnt[f]--;
				 if(wait_cnt[f] ==0){
					wait_cnt[f]	=-1;
					resume(f);
				 }		
			 }  		
		 }
		 father[pid] = -1;
	//END_CHANGE
	if (--numproc == 0)
		xdone();

	freestk(pptr->pbase, pptr->plen);

	pstate = pptr->pstate;
	pptr->pstate = PRFREE;	

	if (pstate == PRCURR)
	  resched();
        else	
         if (pstate == PRWAIT)
         {
            semaph[pptr->psem].semcnt++;
	    dequeue(pid);
         } /* PRWAIT */
         else
         if (pstate == PRREADY)
	    dequeue(pid);
         else
         if (pstate == PRSLEEP)
         {
           if ( q[pid].qnext < NPROC ) /* qnext is not tail */
                q[q[pid].qnext].qkey += q[pid].qkey;  /* Account for the 
                                                              pid delay */
                dequeue(pid);
               /* The sleep queue may now be empty, or have a new first */
            	if ( slnempty = nonempty(clockq) ) 
                         sltop = &firstkey(clockq);

         } /*  PRSLEEP */
         else; /* default: PRSUSP, PRRECV  */
	
	restore(ps);
	return(OK);

} /* kill */
