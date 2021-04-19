/* getmem.c - getmem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>

/*------------------------------------------------------------------------
 *  getmem  --  allocate heap storage, returning lowest integer address
 *------------------------------------------------------------------------
 */
char *getmem(nbytes)
word nbytes;
{
	int	ps;
	struct	mblock	*p, *q, *leftover;
	struct	mblock	*best, *prev;
   word best_size;


   best = NULL;

	disable(ps);
	if ( nbytes==0 ) {
		restore(ps);
		return( NULL );
	}

	nbytes = roundew(nbytes);
	for ( q=&memlist, p=q->mnext ;
		(char *)p != NULL ;
		q=p, p=p->mnext )
		if ( p->mlen == nbytes) { /* if we found an exact one,
                                   we can return */
			q->mnext = p->mnext;
			restore(ps);
			return( (char *) p );
		} else if ( p->mlen > nbytes )
                         if ( (best == ((struct mblock  *) NULL))
                              || (best_size > p->mlen) )
                         {
                           best = p;
                           best_size = p->mlen;
                           prev = q;
                         } /* if */


     if ( best != ((struct mblock *) NULL) )
     /* We found at least one block
                           and mlen > nbytes */
      {
  			leftover = (struct mblock *)( (char *)best + nbytes );
			prev->mnext = leftover;
			leftover->mnext = best->mnext;
			leftover->mlen = best->mlen - nbytes;
			restore(ps);
			return( (char *) best );
      } /* if */

	restore(ps);
	return( NULL );
}
