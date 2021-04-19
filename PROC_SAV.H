/* proc.h - isbadpid */
/* 8088 version */

/* process table declarations and defined constants			*/

#ifndef	NPROC				/* set the number of processes	*/
#define	NPROC		30		/*  allowed if not already done	*/
#endif

/* process state constants */

#define	PRCURR		'\01'		/* process is currently running	*/
#define	PRFREE		'\02'		/* process slot is free		*/
#define	PRREADY		'\03'		/* process is on ready queue	*/
#define	PRRECV		'\04'		/* process waiting for message	*/
#define	PRSLEEP		'\05'		/* process is sleeping		*/
#define	PRSUSP		'\06'		/* process is suspended		*/
#define	PRWAIT		'\07'		/* process is on semaphore queue*/

/* miscellaneous process definitions */

#define	PNMLEN		9		/* length of process "name"	*/
#define	NULLPROC	0		/* id of the null process; it	*/
					/*  is always eligible to run	*/

#define	isbadpid(x)	(x<=0 || x>=NPROC)

/* process table entry */

struct	pentry	{
	char	pstate;			/* process state: PRCURR, etc.	*/
	int	pprio;			/* process priority		*/
	int	psem;			/* semaphore if process waiting	*/
	int	pmsg;			/* message sent to this process	*/
	int	phasmsg;		/* nonzero iff pmsg is valid	*/
	char	*pregs;			/* saved environment		*/
	char	*pbase;			/* base of run time stack	*/
	word	plen;			/* stack length	in bytes	*/
	char	pname[PNMLEN+1];	/* process name			*/
	int	pargs;			/* initial number of arguments	*/
	int	(*paddr)();		/* initial code address		*/
};

extern	struct	pentry proctab[];
extern	int	numproc;		/* currently active processes	*/
extern	int	nextproc;		/* search point for free slot	*/
extern	int	currpid;		/* currently executing process	*/
