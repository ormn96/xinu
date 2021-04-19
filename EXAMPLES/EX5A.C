/* ex5a.c - xmain, prod2, cons2 - busy wait -  wastes CPU time */

#include <conf.h>
#include <kernel.h>

int	n=0;	/* external variables are shared by all processes */
volatile int     produced, consumed;


/*-----------------------------------------------------------------------
 *  xmain  --  producer and consumer processes synchronized with messages
 *-----------------------------------------------------------------------
 */
xmain()
{
	int	prod2(), cons2();

	produced = 1;
	consumed = 0;
        resume(create(cons2,INITSTK,INITPRIO,"cons",0));
        resume(create(prod2,INITSTK,INITPRIO,"prod",0));
} /* xmain */

/*--------------------------------------------------------------------
 *  prod2  --  increment n 2000 times, waiting for it to be consumed
 *--------------------------------------------------------------------
 */
prod2()
{
	int	i, k;

	for (i=1; i<=2000; i++) {
               while(consumed == 0)
                  ;
	       n++;
	       produced = 1;
               consumed = 0;
	}/* for */
} /* prod2 */

/*---------------------------------------------------------------------
 *  cons2  --  print n 2000 times, waiting for it to be produced
 *---------------------------------------------------------------------
 */
cons2()
{
	int	i;

	for (i=1; i<=2000; i++) {
              while(produced == 0)
                  ;
              printf("n is %d\n", n);
              consumed = 1;
	      produced = 0;
	} /* for */
} /* cons2 */
