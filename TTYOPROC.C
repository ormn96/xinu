/* ttyoproc.c - ttyoproc */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <vidio.h>

/*------------------------------------------------------------------------
 *  ttyoproc  --  lower-half tty device driver process for console output
 *------------------------------------------------------------------------
 */
PROCESS	ttyoproc()
{
	register struct	tty   *iptr;
	int	ct;
	int	ps;
	char	ch;
	Bool	enl,onl;
	int	rcvchr();

	iptr = &tty[0];			/* pointer to tty structure	*/
	onl = enl = FALSE;
	disable(ps);
	for (;;) {			/* endless loop for process	*/
		if (enl) {		/* must send linefeed		*/
			enl = FALSE;
			wtty(NEWLINE);
			continue;
		}
		/* look at the echo buffer */
		if ( iptr->ecnt ) {	/* any chars in echo buffer?	*/
			ch = iptr->ebuff[iptr->etail++];
			--iptr->ecnt;
			if (iptr->etail	>= EBUFLEN)
				iptr->etail = 0;
			if ( (ch==RETURN || ch==NEWLINE) && iptr->ecrlf ) {
				enl = TRUE;
				ch = RETURN;
			}
			wtty(ch);
			continue;
		}
		if (iptr->oheld) {
			rcvchr();
			continue;
		}
		if (onl) {		/* must send linefeed		*/
			onl = FALSE;
			wtty(NEWLINE);
			continue;
		}
		if ( (ct=iptr->ocnt) > 0 ) {
			ch = iptr->obuff[iptr->otail++];
			--iptr->ocnt;
			if (iptr->otail	>= OBUFLEN)
				iptr->otail = 0;
			if ( ct < (OBUFLEN-OBMINSP) && iptr->odsend == 0 )
				signal(iptr->osem);
			else if	( ++(iptr->odsend) == OBMINSP ) {
				iptr->odsend = 0;
				signaln(iptr->osem, OBMINSP);
			}
			if ( (ch==RETURN || ch==NEWLINE) && iptr->ocrlf ) {
				onl = TRUE;
				ch = RETURN;
			}
			wtty(ch);
			continue;
		}
		rcvchr();
	}
}

/*------------------------------------------------------------------------
 *  rcvchr  --  wait for another character to arrive
 *------------------------------------------------------------------------
 */
LOCAL rcvchr()
{
	struct	tty	*iiptr;

	if ( winofcur != 0 ) {
		iiptr = &tty[winofcur];
		wputcsr(iiptr,iiptr->curcur);
	}
	if ( receive() == TMSGEFUL ) {
		wtty(BELL);
	}
}
