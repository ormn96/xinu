/* ex11.c - Access how many increments can be done until preemption */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  xmain  --  example of creating processes in PC-Xinu
 *------------------------------------------------------------------------
 */

unsigned long int who_is_current = 0, count = 0;

xmain()
{
    int Inc(), Pr();

    resume( create(Inc, INITSTK, INITPRIO, "proc 1", 0) );
    resume( create(Pr, INITSTK, INITPRIO, "proc 2", 0) );
    while(1)
      ; /* Do nothing but serve as alternative to Inc */
        /* Pr may not be ready (printing) */
}

/*------------------------------------------------------------------------
 *  Inc  --  Increment count
 *------------------------------------------------------------------------
 */
Inc()
{
  int mypid;

  mypid = getpid();  /* Store my pid */
  while (1)
    {
      count = 0;
      who_is_current = mypid; /* Signal I am current */
      /* As long as I am current, increment count */
      while(who_is_current == mypid)
        count++;
     /* I have been replaced and become current again */
    }  /* while */

}  /* Inc */

/*------------------------------------------------------------------------
 *  Pr  --  Print count
 *------------------------------------------------------------------------
 */


Pr()
{
  char str[80];
  int count_value;
  int mypid;

  mypid = getpid();
  while(1)
  {
    count_value = count; /* Store count value aside, locally */
    who_is_current = mypid; /* Signal I am current */
    sprintf(str, "count_value = %lu\n",
                       count_value);
    printf(str);
    while (who_is_current == mypid) /* Do not continue until */
       ;                            /* I am replace and current again */
   } /* while */

} /* Pr */
