/* ttyinit.c - ttyinit */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <bios.h>
#include <kbdio.h>

/*------------------------------------------------------------------------
 *  ttyinit  --  initialize buffers and modes for a tty line
 *------------------------------------------------------------------------
 */
ttyinit(devptr)
	struct	devsw	*devptr;
{
	register struct	tty *iptr;
	char	*cp;
	int	pid;
	int	ttyoproc();
	int	ttyiproc();

	iptr = &tty[devptr->dvminor];
	devptr->dvioblk = (char *) iptr;	/* fill tty control blk	*/

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
	iptr->ierase = iptr->ieback = TRUE;/* console honors erase	*/
	iptr->ierasec = BACKSP;			/*  using ^h		*/
	iptr->ecrlf = iptr->icrlf = TRUE;	/* map RETURN on input	*/
	iptr->ocrlf = iptr->oflow = TRUE;
	iptr->ikill = TRUE;		/* set line kill == @		*/
	iptr->ikillc = ATSIGN;
	iptr->oheld = FALSE;
	iptr->ostart = STRTCH;
	iptr->ostop = STOPCH;
	iptr->icursor = 0;
	iptr->ifullc = TFULLC;
	iptr->curcur.row = 0;
	iptr->curcur.col = 0;

	/* now set up new tty process for this device */
	pid = create(ttyoproc,INITSTK,TTYOPRIO,"*TTYO*",0);
	if ( pid == SYSERR )
		kprintf("Can't create console output process\n");
	else
		ready(pid);
	iptr->oprocnum = pid;
	pid = create(ttyiproc,INITSTK,TTYIPRIO,"*TTYI*",0);
	if ( pid == SYSERR )
		kprintf("Can't create console input process\n");
	else
		ready(pid);
	iptr->wstate = pid;	
}
