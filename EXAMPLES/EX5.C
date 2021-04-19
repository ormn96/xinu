/* ex5.c - xmain, prod2, cons2 */

#include <conf.h>
#include <kernel.h>

int	n=0;		/* external variables are shared by all processes */

/*------------------------------------------------------------------------
 *  xmain  --  producer and consumer processes synchronized with semaphores
 *------------------------------------------------------------------------
 */
xmain()
{
	int	prod2(), cons2();
	int	produced, consumed;

	consumed = screate(0);
	produced = screate(1);
	resume(create(prod2,INITSTK,INITPRIO,"prod",2,consumed,produced));
	resume(create(cons2,INITSTK,INITPRIO,"cons",2,consumed,produced));

}

/*------------------------------------------------------------------------
 *  prod2  --  increment n 2000 times, waiting for it to be consumed
 *------------------------------------------------------------------------
 */
prod2(consumed,produced)
int consumed,produced;
{
	int	i;

	for (i=1; i<=2000; i++) {
		wait(consumed);
		n++;
		signal(produced);
	}
}

/*------------------------------------------------------------------------
 *  cons2  --  print n 2000 times, waiting for it to be produced
 *------------------------------------------------------------------------
 */
cons2(consumed,produced)
int consumed,produced;
{
	int	i;

	for (i=1; i<=2000; i++) {
		wait(produced);
		printf("n is %d\n",n);
		signal(consumed);
	}
}
