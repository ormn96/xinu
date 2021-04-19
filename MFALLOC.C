/* mfalloc.c - mfalloc */

#include <conf.h>
#include <kernel.h>
#include <mffile.h>

/*------------------------------------------------------------------------
 *  mfalloc  --  allocate a device table entry for a dos file; return id
 *------------------------------------------------------------------------
 */
#ifdef	Nmf
mfalloc()		/* assume ints disabled, provided by caller	*/
{
	int	i;

	for (i=0 ; i<Nmf ; i++)
		if (mftab[i].mf_pid == 0) {
			mftab[i].mf_pid = getpid();
			return(i);
		}
	return(SYSERR);
}
#endif
