/* mutex.c */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <sem.h>
#include <q.h>
#include <proc.h>
#include "mutex.h"

SYSCALL mutex_lock(MUTEX_REC_PTR *mutex_var)
{
 int nsem;
 int ps, ans;

 disable(ps);

  if ((*mutex_var) == (MUTEX_REC_PTR)NULL)
  {
    nsem = screate(1);
    if (nsem == SYSERR)
    {
      restore(ps);
      return SYSERR;
    } /* if */
    (*mutex_var) = (MUTEX_REC_PTR)getmem(sizeof(MUTEX_REC));
    (*mutex_var)->nsem = nsem;

  } /* if */


    ans = wait((*mutex_var)->nsem);
    (*mutex_var)->pid = currpid;
    restore(ps);
    return  ans;

} /* mutex_lock */

SYSCALL mutex_unlock(MUTEX_REC_PTR *mutex_var)
{
 int ps;

  disable(ps);
  if ( ((*mutex_var) == (MUTEX_REC_PTR)NULL) ||
     ((*mutex_var)->pid != currpid) )
   {
     restore(ps);
     return SYSERR;
   } // if

    if ( isempty(semaph[(*mutex_var)->nsem].sqhead))
      {
       sdelete((*mutex_var)->nsem);
       freemem((*mutex_var), sizeof(MUTEX_REC));
       (*mutex_var) = MUTEX_VAR_INITIALIZER;
       restore(ps);
       return OK;
      } /* if */

    signal((*mutex_var)->nsem);

  restore(ps);
  return OK; 

} /* mutex_unlock */
