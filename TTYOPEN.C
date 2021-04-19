/* ttyopen.c - ttyopen */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <tty.h>
#include <io.h>
#include <bios.h>

/*------------------------------------------------------------------------
 *  ttyopen  --  open a window on a tty
 *------------------------------------------------------------------------
 */
ttyopen(devptr,bp,ap)
struct	devsw	*devptr;
char *bp;				/* border string		*/
char *ap;				/* attribute string		*/
{
	register struct	tty *iptr;
	int	pid;
	int	ps;
	char	cp[7];		/* output server process name		*/
	int 	i;
	CURSOR	boxp[2];	/* window corners from passed params	*/
	CURSOR	topl,botr;	/* topleft, bottom right of window	*/
	int	attr;		/* window attributes (color, etc.)	*/
	int	lwoproc();	/* tty server process			*/
	int	brdr;		/* true if the window has a border	*/

	if ( bp == NULL || *bp == 0 )	/* reopen the console		*/
		return(tty[0].dnum);
	if ( (brdr=lwbord(bp,boxp))==SYSERR || (attr=lwattr(ap,BW))==SYSERR )
		return(SYSERR);
	topl = boxp[0];
	botr = boxp[1];
	if ( topl.col >= G_COLS || topl.row >= G_ROWS
		|| botr.col >= G_COLS || botr.row >= G_ROWS )
		return(SYSERR);
	i = brdr ? 2 : 0;		/* offset for border		*/
	if ( topl.row+i > botr.row || topl.col+i > botr.col )
		return(SYSERR);		/* not enough room for window	*/
	if ( brdr ) {			/* make room for the border	*/
		botr.col--;
		botr.row--;
		topl.col++;
		topl.row++;
	}
	disable(ps);
	if ( (i=wfree()) == SYSERR ) {
		restore(ps);
		return(SYSERR);
	}
	iptr = &tty[i];
	strcpy(cp,"*LWO *");
	cp[4] = '0'+i;
	if ( (pid=create(lwoproc,INITSTK,TTYOPRIO,cp,1,i)) == SYSERR ) {
		iptr->oprocnum = -1;
		iptr->wstate = LWFREE;	/* mark it as free 		*/
		restore(ps);
		return(SYSERR);
	}
	iptr->oprocnum = pid;
	ready(pid);
	iptr->hasborder = brdr;
	iptr->topleft = topl;
	iptr->botright = botr;
	iptr->attr = attr;
	iptr->rowsiz = botr.row-topl.row+1;
	iptr->colsiz = botr.col-topl.col+1;
	iptr->curcur.row = 0;
	iptr->curcur.col = 0;
	iptr->ihead = iptr->itail = 0;		/* empty input queue	*/
	iptr->isem = screate(0);		/* chars. read so far=0	*/
	iptr->icnt = 0;
	iptr->osem = screate(OBUFLEN);		/* buffer available=all	*/
	iptr->odsend = 0;			/* sends delayed so far	*/
	iptr->ohead = iptr->otail = 0;		/* output queue empty	*/
	iptr->ocnt = 0;
	iptr->ehead = iptr->etail = 0;		/* echo queue empty	*/
	iptr->ecnt = 0;
	iptr->imode = IMCOOKED;
	iptr->iecho = iptr->evis = TRUE;	/* echo console input	*/
	iptr->ierase = iptr->ieback = TRUE;	/* console honors erase	*/
	iptr->ierasec = BACKSP;			/*  using ^h		*/
	iptr->ecrlf = iptr->icrlf = TRUE;	/* map RETURN on input	*/
	iptr->ocrlf = iptr->oflow = TRUE;
	iptr->ikill = TRUE;			/* set line kill == @	*/
	iptr->ikillc = ATSIGN;
	iptr->oheld = FALSE;
	iptr->ostart = STRTCH;
	iptr->ostop = STOPCH;
	iptr->icursor = 0;
	iptr->ifullc = TFULLC;
	scrollup(boxp[0],boxp[1],0,attr); /* erase the window		*/
	if ( brdr )
		border(boxp[0],boxp[1]); /* draw the border		*/
	restore(ps);
	return(iptr->dnum);
}

/*------------------------------------------------------------------------
 *  border  --  draw a border around a screen window
 *------------------------------------------------------------------------
 */
LOCAL border(tl,br)
CURSOR tl,br;
{
	CURSOR	csr;		/* used for absolute cursor positioning	*/
	int	pcx;

	xdisable(pcx);
	for ( csr.row=tl.row; csr.row<=br.row; csr.row++ ) {
		if (csr.row==tl.row) {
			csr.col = tl.col;
			putcsrpos(csr,0);
			putchr(TLBORDER,1,0);
			for ( csr.col++ ; csr.col<br.col ; csr.col++ ) {
				putcsrpos(csr,0);
				putchr(HBORDER,1,0);
			}
			putcsrpos(csr,0);
			putchr(TRBORDER,1,0);
		} else if (csr.row==br.row) {
			csr.col = tl.col;
			putcsrpos(csr,0);
			putchr(BLBORDER,1,0);
			for ( csr.col++ ; csr.col<br.col ; csr.col++ ) {
				putcsrpos(csr,0);
				putchr(HBORDER,1,0);
			}
			putcsrpos(csr,0);
			putchr(BRBORDER,1,0);
		} else {
			csr.col = tl.col;
			putcsrpos(csr,0);
			putchr(VBORDER,1,0);
			csr.col = br.col;
			putcsrpos(csr,0);
			putchr(VBORDER,1,0);
		}
	}
	xrestore(pcx);
}

/*------------------------------------------------------------------------
 *  wfree  --  get a free window slot
 *------------------------------------------------------------------------
 */
LOCAL
wfree()
{
	int	i;
	struct tty	*iptr;

	for ( i=1 ; i<Ntty ; i++ ) {
		iptr = &tty[i];
		if ( iptr->wstate == LWFREE ) {
			iptr->wstate = LWALLOC;
			iptr->seq++;
			return(i);
		}
	}
	return(SYSERR);
}
