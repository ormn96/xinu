/* userret.c - userret */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  userret  --  entered when a process exits by return
 *------------------------------------------------------------------------
 */
userret()
{
	int	pid;

	kill( pid=getpid() );
	kprintf("Fatal system error - unable to kill process %d",pid);
}
