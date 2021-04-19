/* testmu2.c - xmain */

#include <kernel.h>

#include <conf.h>
#include <proc.h>
#include <q.h>
#include "mutex.h"

/*------------------------------------------------------------------------
 *  xmain  --  write "hi" on the CONSOLE
 *------------------------------------------------------------------------
 */

volatile int bank[2] = {450, 600};

MUTEX_VAR mutex1 = MUTEX_VAR_INITIALIZER;

int bank1()
{
 int sums[] = {100, -50, 150, -200};

 int i=0, j, sum;

 while(1)
  {
   i++;
   i = i % 4;
   sum = sums[i];
   mutex_lock(&mutex1);
   bank[0] += sum;
   bank[1] -= sum;
   mutex_unlock(&mutex1);

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
   mutex_lock(&mutex1);
   sum = bank[0] + bank[1];
   mutex_unlock(&mutex1);
   if ((sum != 1050)|| (i == 0))
     printf("sum = %d\n", sum);
  } /* while */

} /* bank1 */

xmain()
{

  resume(create(bank1,INITSTK, INITPRIO, "proc 1", 0) );
  resume(create(bank2,INITSTK, INITPRIO, "proc 2", 0) );
} /* xmain */

