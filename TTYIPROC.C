/* ttyiproc.c - ttyiproc, erase1, eputc, echoch */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <bios.h>
#include <butler.h>
#include <kbdio.h>

/*------------------------------------------------------------------------
 *  ttyiproc  --  lower-half tty device process for input characters
 *------------------------------------------------------------------------
 */
PROCESS ttyiproc()
{
	register struct	tty	*iptr;	/* pointer to tty block		*/
	register int	ch;
	int	ct,w;
	int	ps;

	disable(ps);
	for ( ;; ) {
		if ( (ch=kbdgetc()) == NOCH ) {
			receive();
			continue;
		}
		if ( ch >= SPECKEY ) {
			if ( ch == CFKEY+PSNAPK ) { /* process snapshot	*/
				send(butlerpid,MSGPSNAP);
				continue;
			}
			if ( ch == CFKEY+TSNAPK ) { /* tty snapshot	*/
				send(butlerpid,MSGTSNAP);
				continue;
			}
			if ( ch == CFKEY+DSNAPK ) { /* disk snapshot	*/
				send(butlerpid,MSGDSNAP);
				continue;
			}
			if ( ch >= FKEY && ch < FKEY+10 ) { /* window?	*/
					/* F10 maps to minor device 0	*/
				if ( (w=ch-FKEY+1) == 10 )
					w = 0;
				if ( w < Ntty ) {
					iptr = &tty[w];
					if ( iptr->wstate > 0 ) {
						winofcur = w;
						send(iptr->oprocnum,TMSGOK);
					}
				}
				continue;
			}
		}
		iptr = &tty[winofcur];	/* get pointer to tty entry	*/
		if (iptr->imode == IMRAW) {
			if ( iptr->icnt >= IBUFLEN )
				continue;
			iptr->ibuff[iptr->ihead++] = ch;
			++iptr->icnt;
			if (iptr->ihead	>= IBUFLEN)
				iptr->ihead = 0;
			signal(iptr->isem);
			continue;
		}
		/* cbreak | cooked mode	*/
		if ( ch	== RETURN && iptr->icrlf )
			ch = NEWLINE;
		if (iptr->oflow) {
			if (ch == iptr->ostart)	{
				iptr->oheld = FALSE;
				send(iptr->oprocnum,TMSGOK);
				continue;
			}
			if (ch == iptr->ostop) {
				iptr->oheld = TRUE;
				continue;
			}
		}
		iptr->oheld = FALSE;
		if (iptr->imode	== IMCBREAK) {		/* cbreak mode	*/
			if ( iptr->icnt >= IBUFLEN ) {
				if (iptr->iecho)
					eputc(iptr->ifullc,iptr);
				continue;
			}
			iptr->ibuff[iptr->ihead++] = ch;
			++iptr->icnt;
			if (iptr->ihead	>= IBUFLEN)
				iptr->ihead = 0;
			echoch(ch,iptr);
			signal(iptr->isem);
			continue;
		}
		/* cooked mode	*/
		if (ch == iptr->ikillc && iptr->ikill) {
			iptr->ihead -= iptr->icursor;
			iptr->icnt -= iptr->icursor;
			if ( iptr->ihead < 0 )
				iptr->ihead += IBUFLEN;
			iptr->icursor =	0;
			if (iptr->iecho)
				eputc(NEWLINE,iptr);
			continue;
		}
		if (ch == iptr->ierasec	&& iptr->ierase) {
			if (iptr->icursor > 0) {
				--iptr->icursor;
				if ( --(iptr->ihead) < 0 )
					iptr->ihead += IBUFLEN;
				--iptr->icnt;
				erase1(iptr);
			}
			continue;
		}
		if ( (ch==NEWLINE || ch==RETURN) && iptr->icnt < IBUFLEN ) {
			echoch(ch,iptr);
			iptr->ibuff[iptr->ihead++] = ch;
			++iptr->icnt;
			if (iptr->ihead	>= IBUFLEN)
				iptr->ihead = 0;
			ct = iptr->icursor+1; /* +1 for \n or \r*/
			iptr->icursor =	0;
			signaln(iptr->isem,ct);
			continue;
		}
		if ( iptr->icnt >= IBUFLEN-1) {
			if (iptr->iecho)
				eputc(iptr->ifullc,iptr);
			continue;
		}
		echoch(ch,iptr);
		iptr->icursor++;
		iptr->ibuff[iptr->ihead++] = ch;
		++iptr->icnt;
		if (iptr->ihead	>= IBUFLEN)
			iptr->ihead = 0;
	} /* end of forever loop */
}

/*------------------------------------------------------------------------
 *  erase1  --  erase one character honoring erasing backspace
 *------------------------------------------------------------------------
 */
LOCAL erase1(iptr)
struct tty *iptr;
{
	char	ch;

	if ( iptr->iecho == 0 )
		return;
	ch = iptr->ibuff[iptr->ihead];
	if ( (ch<BLANK || ch==0177) && iptr->evis ) {
		eputc(BACKSP,iptr);
		if (iptr->ieback) {
			eputc(BLANK,iptr);
			eputc(BACKSP,iptr);
		}
	}
	eputc(BACKSP,iptr);
	if (iptr->ieback) {
		eputc(BLANK,iptr);
		eputc(BACKSP,iptr);
	}
}

/*------------------------------------------------------------------------
 *  echoch  --  echo a character with visual option
 *------------------------------------------------------------------------
 */
LOCAL echoch(ch, iptr)
char ch;				/* character to	echo		*/
struct tty *iptr;			/* ptr to I/O block		*/
{
	if ( iptr->iecho == 0 )
		return;			/* nothing to do		*/
	if ( ch==NEWLINE || ch==RETURN || ch==TAB || ch==BELL ) {
		eputc(ch,iptr);
		return;
	}
	if ( (ch<BLANK || ch==0177) && iptr->evis ) {
		eputc(UPARROW,iptr);
		eputc(ch+0100,iptr);	/* make it printable	*/
		return;
	}
	eputc(ch,iptr);
}

/*------------------------------------------------------------------------
 *  eputc  --  put one character in the echo queue
 *------------------------------------------------------------------------
 */
LOCAL eputc(ch,iptr)
char ch;
struct tty *iptr;
{
	if ( iptr->ecnt < EBUFLEN ) {
		iptr->ebuff[iptr->ehead++] = ch;
		++iptr->ecnt;
		if (iptr->ehead	>= EBUFLEN)
			iptr->ehead = 0;
		send(iptr->oprocnum,TMSGOK);
		return;
	}
	sendf(iptr->oprocnum,TMSGEFUL);	/* wake it up!!! */
}
