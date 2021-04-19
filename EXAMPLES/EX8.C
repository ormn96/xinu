/* ex8.c - xmain, Inc1, Inc2, Inc3, Pr */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  xmain  --  example of creating processes in PC-Xinu
 *------------------------------------------------------------------------
 */

int count1 = 0, count2 = 0, count3 = 0;

xmain()
{
    int Inc(), Pr();

    resume( create(Inc, INITSTK, INITPRIO, "proc 1", 1, &count1) );
    resume( create(Inc, INITSTK, INITPRIO, "proc 2", 1, &count2) );
    resume( create(Inc, INITSTK, INITPRIO - 1, "proc 3", 1, &count3) );
    resume( create(Pr, INITSTK, INITPRIO + 1, "proc 4", 0) );
}

/*------------------------------------------------------------------------
 *  Inc  --  Increment counter via pointer
 *------------------------------------------------------------------------
 */
Inc(int ptr)
{
  int *ptr1;

  ptr1 = (int *)ptr;
  while (1)
     (*ptr1)++;

}  /* Inc */

Pr()
{
  while(1)
  {
    sleep(3);
    printf("count1 = %d, count2 = %d, count3 = %d\n",
                       count1, count2, count3);
   } /* while */

} /* Pr */
