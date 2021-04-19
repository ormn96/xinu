/* xdone.c - xdone */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <io.h>
#include <disk.h>
#include <tty.h>

/*------------------------------------------------------------------------
 *  xdone  --  print system termination message and terminate PC-Xinu
 *------------------------------------------------------------------------
 */
int xdone()
{
	int	kprintf();
	int	ps;
	int	i;

#ifdef Ndsk
	for ( i=0; i<Ndsk; i++ )
		control(dstab[i].dnum,DSKSYNC);	/* sync the disks	*/
#endif
	sleep(1);			/* let tty output settle	*/
	disable(ps);
	maprestore();			/* restore interrupt vectors	*/
	restore(ps);
	kprintf("\n\n-- system halt --\n\n");
	if (numproc==0)
		kprintf("All user processes have completed\n");
	else
		kprintf("PC-Xinu terminated with %d process%s active\n",
			numproc, ((numproc==1) ? "" : "es"));
	kprintf("Returning . . .\n\n");
	halt();				/* return to caller		*/
}
