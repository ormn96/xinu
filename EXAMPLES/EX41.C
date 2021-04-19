/* ex4.c - xmain, produce, consume */

#include <conf.h>
#include <kernel.h>

int	n=0;		/* external variables are shared by all processes */

/*------------------------------------------------------------------------
 *  xmain  --  example of unsynchronized producer and consumer processes
 *------------------------------------------------------------------------
 */
xmain()
{
	int	produce(), consume();

        resume( create(consume,INITSTK,INITPRIO,"cons",0) );
        resume( create(produce,INITSTK,INITPRIO,"prod",0) );
}

/*------------------------------------------------------------------------
 *  produce  --  increment n 2000 times and exit
 *------------------------------------------------------------------------
 */
produce()
{
	int	i;

	for (i=1; i<=2000; i++)
		n++;
}

/*------------------------------------------------------------------------
 *  consume  --  print n 2000 times and exit
 *------------------------------------------------------------------------
 */
consume()
{
	int	i;

	for (i=1; i<=2000; i++)
		printf("n is %d\n",n);
}
