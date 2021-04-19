/* mem.h - roundew, truncew, getstk, freestk */

/*----------------------------------------------------------------------
 * roundew, truncew - round or truncate address to next even word 
 *----------------------------------------------------------------------
 */
#define	roundew(x)	( (3 + (WORD)(x)) & (~3) )
#define	truncew(x)	( ((WORD)(x)) & (~3) )

#define getstk(n)	getmem(n)
#define freestk(b,s)	freemem(b,s)

typedef struct	mblock	{
	struct	mblock	*mnext;
	word	mlen;
} MBLOCK;

#define	end	endaddr			/* avoid C library conflict	*/

extern	MBLOCK	memlist;		/* head of free memory list	*/
extern	char	*maxaddr;		/* max memory address		*/
extern	char	*end;			/* address beyond loaded memory	*/

#define MMAX	65024			/* maximum memory size		*/
#define	MBLK	512			/* block size for global alloc	*/
#define	MMIN	8192			/* minimum Xinu allocation	*/
#define MDOS	1024			/* save something for MS-DOS	*/

extern	char	*getmem();
extern	int	freemem();
