/* mscntl.c - mscntl */

#include <conf.h>
#include <kernel.h>
#include <mffile.h>

/*------------------------------------------------------------------------
 *  mscntl  --  ioctl for the MS-DOS file system
 *------------------------------------------------------------------------
 */
int mscntl(devptr, funct, arg)
struct devsw *devptr;
int funct;
char *arg;
{
	return(OK);
}
