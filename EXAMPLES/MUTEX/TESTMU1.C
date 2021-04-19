/* testmu1.c - xmain */
#include <kernel.h>

#include <conf.h>
#include <proc.h>
#include <q.h>

/*------------------------------------------------------------------------
 *  xmain  --  write "hi" on the CONSOLE
 *------------------------------------------------------------------------
 */

volatile int bank[2] = {450, 600};

int bank1()
{
 int sums[] = {100, -50, 150, -200};

 int i=0, j, sum;


 while(1)
  {
   i++;
   i = i % 4;
   sum = sums[i];
   bank[0] += sum;
   for(j=0; j < 32000; j++)
        ;
   bank[1] -= sum;

  } /* while */

} /* bank1 */

int bank2()
{

 int sum;
 int i= 0;

 while(1)
  {
   i++;
   i = i % 30000;
   sum = bank[0] + bank[1];
   if ((sum != 1050)|| (i == 0))
     printf("sum = %d\n", sum);
  } /* while */

} /* bank1 */

xmain()
{

  resume(create(bank1,INITSTK, INITPRIO, "proc 1", 0) );
  resume(create(bank2,INITSTK, INITPRIO, "proc 2", 0) );
} /* xmain */

