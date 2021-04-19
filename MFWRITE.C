/* mfwrite.c - mfwrite */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  mfwrite  --  write 'count' bytes onto a local disk file
 *------------------------------------------------------------------------
 */
mfwrite(devptr, buff, count)
struct	devsw	*devptr;
char	*buff;
int	count;
{
	int	i;

	if (count < 0)
		return(SYSERR);
	for (i=count; i>0 ; i--)
		if (mfputc(devptr, *buff++) == SYSERR)
			return(SYSERR);
	return(count);
}
