/* panic.c - panic, _panic */

#include <dos.h>
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <io.h>
#include <tty.h>
#include <bios.h>

/*------------------------------------------------------------------------
 *  panic  --  print error message and terminate PC-Xinu
 *------------------------------------------------------------------------
 */
int panic(cp)
char *cp;
{
	int i;
	int kprintf();
	int	ps;

	disable(ps);
	maprestore();			/* restore interrupts to DOS	*/
	restore(ps);
	kprintf("\n\n-- panic stop --\n%s\n\n",cp);
	if (numproc==0)
		kprintf("All user processes have completed\n");
	else
		kprintf("PC-Xinu terminated with %d process%s active\n",
			numproc, ((numproc==1) ? "" : "es"));
	kprintf("Returning . . .\n\n");
	halt();				/* return to caller		*/
}

#define MAXSPRINT	6		/* max stack values to print	*/

static int i;				/* counter for printing stack	*/
static char *ep;			/* error message pointer	*/
static struct pentry *pp;		/* pointer to this proc. entry	*/
static int sssave, spsave;		/* user stack segment, pointer	*/
static int far *sp;			/* pointer to original stack	*/

/*------------------------------------------------------------------------
 *  _panic  --  exception handler, called by exception interrupts
 *------------------------------------------------------------------------
 */
_panic(typ)
int typ;				/* exception type		*/
{
	maprestore();
	switch (typ) {
	case DB0VEC:
		ep = "Divide by zero";
		break;
	case SSTEPVEC:
		ep = "Single step";
		break;
	case BKPTVEC:
		ep = "Breakpoint";
		break;
	case OFLOWVEC:
		ep = "Arithmetic overflow";
		break;
	default:
		ep = "Unknown interrupt";
	}
	kprintf("\n\n-- panic stop -- \n");
	kprintf("panic: trap type %d (%s) \n",typ,ep);
	pp = &proctab[currpid];
	kprintf("process pid=%d name=%s \n",currpid,pp->pname);
	sys_getstk(&sssave,&spsave);	/* retrieve stack parameters	*/
	FP_SEG(sp) = sssave;
	FP_OFF(sp) = spsave;
	sp += 13;			/* actual top of user stack	*/
	kprintf("state: ax=%04x bx=%04x cx=%04x dx=%04x",
		*(sp-6),*(sp-7),*(sp-8),*(sp-9));
	kprintf(" si=%04x di=%04x bp=%04x sp=%04x \n",
		*(sp-10),*(sp-11),*(sp-5),FP_OFF(sp));
	kprintf("       cs=%04x ds=%04x ss=%04x es=%04x",
		*(sp-2),*(sp-12),sssave,*(sp-13));
	kprintf(" ip=%04x fl=%04x \n",*(sp-3),*(sp-1));
	kprintf("stack dump:      \n");
	for ( i=0 ; i<MAXSPRINT ; i++, sp++ )
		kprintf("       sp=%04x *sp=%04x \n",FP_OFF(sp),*sp);
	kprintf("\nPC-Xinu terminated with %d process%s active \n",
		numproc, ((numproc==1) ? "" : "es"));
	kprintf("Returning . . . \n\n");
	halt();				/* return to caller		*/
}
