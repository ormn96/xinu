/* tsnap.c - tsnap */

/*------------------------------------------------------------------------
 *  tsnap  --  print a summary of tty statistics
 *------------------------------------------------------------------------
 */
 
#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <sem.h>

static struct	tsnap {
	int	ihead;			/* input queue head		*/
	int	itail;			/* input queue tail		*/
	int	isem;			/* input semaphore value	*/
	int	icnt;			/* input count			*/
	int	ohead;			/* output queue head		*/
	int	otail;			/* output queue tail		*/
	int	osem;			/* output semaphore value	*/
	int	ocnt;			/* output count			*/
	int	odsnd;			/* output defer count		*/
	int	ehead;			/* echo queue head		*/
	int	etail;			/* echo queue tail		*/
	int	ecnt;			/* echo count			*/
	int	wstate;
} ttab[Ntty];

tsnap()
{
	int	i,ps;
	struct	tsnap	*tabp;
	struct	tty	*ttyp;

	kprintf("\ncurrent window = %d\n",winofcur);
	kprintf("%2s %5s %5s %5s %5s  %5s %5s %5s %5s %5s  %5s %5s %5s\n",
		"wn","ihead","itail"," isem"," icnt","ohead","otail"," osem",
		" ocnt","odsnd","ehead","etail"," ecnt");
	disable(ps);
	for (i=0 ; i<Ntty ; i++) {
		ttyp = &tty[i];
		tabp = &ttab[i];
		if ( (tabp->wstate=ttyp->wstate) > 0 ) {
			tabp->ihead = ttyp->ihead;
			tabp->itail = ttyp->itail;
			tabp->isem  = semaph[ttyp->isem].semcnt;
			tabp->icnt  = ttyp->icnt;
			tabp->ohead = ttyp->ohead;
			tabp->otail = ttyp->otail;
			tabp->osem  = semaph[ttyp->osem].semcnt;
			tabp->ocnt  = ttyp->ocnt;
			tabp->odsnd = ttyp->odsend;
			tabp->ehead = ttyp->ehead;
			tabp->etail = ttyp->etail;
			tabp->ecnt  = ttyp->ecnt;
		}
	}
	restore(ps);
	for (i=0 ; i<Ntty ; i++) {
		tabp = &ttab[i];
		if ( tabp->wstate > 0 )
		kprintf("%2d %5d %5d %5d %5d  %5d %5d %5d %5d %5d  %5d %5d %5d\n",
			i,tabp->ihead,tabp->itail,tabp->isem,tabp->icnt,
			tabp->ohead,tabp->otail,tabp->osem,tabp->ocnt,
			tabp->odsnd,tabp->ehead,tabp->etail,tabp->ecnt);
	}		
}
