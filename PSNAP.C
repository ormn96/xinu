/* psnap.c - psnap */

/*------------------------------------------------------------------------
 *  psnap  --  print a snapshot of the process table
 *------------------------------------------------------------------------
 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>

#define CTRLC	3			/* ASCII code for control-C	*/

char *sttab[8] = {
	"???",
	"current",
	"free",
	"ready",
	"receiving",
	"sleeping ",
	"suspended",
	"waiting"
};

#define STATE(s) sttab[(s>0&&s<8)?s:0]

static struct psnap {
	int	state;
	int	prio;
	char	name[10];
	char	*pbase;
	int	plen;
	char	*pregs;
	int	ctx[4];
} stab[NPROC];

static struct qent qtab[NQENT];
	
static int pmark[NPROC];
static char qname[4];

#define next(i)	qtab[(i)].qnext
#define prev(i) qtab[(i)].qprev
#define key(i)	qtab[(i)].qkey

int psnap()
{
	int	i,j;
	int	s;
	int	ps;
	register struct	pentry *pptr;
	register struct	psnap *sptr;
	int	*addr;

	disable(ps);
	/* retrieve a copy of the process table */
	for ( i=0; i<NPROC; i++ ) {
		pptr = &proctab[i];
		sptr = &stab[i];
		sptr->state = pptr->pstate;
		sptr->prio = pptr->pprio;
		strcpy(sptr->name,pptr->pname);
		sptr->pbase = pptr->pbase;
		sptr->plen = pptr->plen;
		sptr->pregs = pptr->pregs;
		if ( pptr->pstate != PRCURR ) {
			addr = (int *) pptr->pregs;
			for ( j=0 ; j<4 ; j++ )
				sptr->ctx[j] = *addr++;
		}
	}
	/* retrieve a copy of the q structure */
	for ( i=0; i<NQENT; i++ )
		qtab[i] = q[i];
	restore(ps);

	/* now print the process table */
	pprint();
	
	kprintf("Press any key to continue . . .");
	if ( kgetc(0) == CTRLC )
		return;
	kprintf("\n");

	/* now print the queues */
	for ( i=0; i<NPROC; i++ )
		pmark[i] = 0;
	/* print the semaphore queues */
	i = NPROC;
	for ( j=0; j<NSEM; j++ ) {
		qname[0] = 'S';
		qname[1] = '0'+(j/10);
		qname[2] = '0'+(j%10);
		qname[3] = 0;
		qprint(i,qname,PRWAIT,0);
		i += 2;
	}
	qprint(i,"RDY",PRREADY,1);
	i += 2;
	qprint(i,"SLP",PRSLEEP,1);
	for ( i=0 ; i<NPROC; i++ )
		if ( pmark[i] == 0 ) {
			s = stab[i].state;
			if ( s == PRWAIT || s == PRREADY || s == PRSLEEP )
				kprintf("?%02d: %s %s\n",i,stab[i].name,STATE(s));
		}
}

static pprint()
{
	int	i,j;
	int	s;
	struct	pentry *pptr;
	struct	psnap *sptr;

	kprintf("\n id state      prio name        sp base  top");
	kprintf("   di   si flag   bp\n");
	for ( i=0; i<NPROC; i++ ) {
		sptr = &stab[i];	/* pointer to process entry	*/
		if ( (s=sptr->state) != PRFREE ) {
			kprintf("%3d %-9s %5d %s",
				i,STATE(s),
				sptr->prio,sptr->name);
			kprintf(" %04x %04x %04x",(int)sptr->pregs,
				(int)sptr->pbase,sptr->plen+(int)sptr->pbase);
			if ( s != PRCURR )
				for (j=0; j<4; j++ )
					kprintf(" %04x",sptr->ctx[j]);
			kprintf("\n");
		}
	}
}

static qprint(i,qnam,state,keyprint)
int i;
char *qnam;
int state;
int keyprint;
{
	int	ni,pi;
	int	s;

	if ( prev(i) != EMPTY )
		kprintf("%s: nonempty qprev header\n",qnam);
	if ( next(i+1) != EMPTY )
		kprintf("%s: nonempty qnext header\n",qnam);
	for ( ni=next(i), pi=i ; ni < NPROC ; pi=ni, ni=next(ni) ) {
		kprintf("%s: pid=%02d",qnam,ni);
		if ( ni < 0 ) {
			kprintf(" ?queue index corrupted!\n");
			break;
		}
		kprintf(" %s",stab[ni].name);
		if ( keyprint )
			kprintf(" key=%d",key(ni));
		if ( pmark[ni]++ != 0 ) {
			kprintf(" ?process on another queue!\n");
			break;
		}
		s = stab[ni].state;
		if ( s != state )
			kprintf(" ?illegal state (%d=%s)",s,STATE(s));
		if ( prev(ni) != pi )
			kprintf(" ?invalid qprev index (%d)",prev(ni));
		kprintf("\n");
	}
	if ( prev(ni) != pi ) {
		kprintf("%s: ?invalid qprev index (%d)",qnam,prev(ni));
	}
	if ( ni != i+1 )
		kprintf("%s: ?queue terminates incorrectly (%d)",qnam,ni);
	if ( pi != i )
		kprintf("----\n");
}
