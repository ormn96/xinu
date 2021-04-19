/* ex9.c - xmain, Inc1, Inc2, Inc3, Pr */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  xmain  --  example of creating processes in PC-Xinu
 *------------------------------------------------------------------------
 */

unsigned long int count1 = 0, count2 = 0, count3 = 0;

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
  unsigned long int *ptr1;

  ptr1 = (unsigned long int *)ptr;
  while (1)
     (*ptr1)++;

}  /* Inc */

Pr()
{
  char str[80];

  while(1)
  {
    sleep(3);
    sprintf(str, "count1 = %lu, count2 = %lu, count3 = %lu\n",
                       count1, count2, count3);
    printf(str);
   } /* while */

} /* Pr */
