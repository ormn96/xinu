 /* xexecl.c - xmain, prA, prB */

#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <q.h>
#include <bios.h>
#include <kbdio.h>

#define INITF 0x0200

extern int INITRET();

/*------------------------------------------------------------------------
 *  xmain  --  example of creating processes in PC-Xinu
 *------------------------------------------------------------------------
 */

void prA(), prB();

xmain()
{

	resume( create(prA, INITSTK, INITPRIO, "proc 1", 1,  'A') );
}

/* xexecl - emulate unix execl in xinu */ 

xexecl(void (*pf)(), int parm)
{
struct pentry *pptr;
char *saddr;
int *sp1;
int ps;
int dummy;

disable(ps);
pptr = &proctab[currpid];
pptr->phasmsg = 0;
sp1 = (int *)(pptr->pbase + pptr->plen);
pptr->pargs = 1;
*(--sp1) = parm;
*(--sp1) = (int) INITRET;
*(--sp1)= (int) pf;
--sp1;
*(--sp1) = INITF;
sp1 -= 2;
pptr->pregs = sp1;
pptr->paddr = pf;

ctxsw(&dummy, &pptr->pregs);

} /* xexecl */

/*------------------------------------------------------------------------
 *  prA  --  repeatedly print 'A' without ever terminating
 *------------------------------------------------------------------------
 */

void prA(int ch)
{
  int i;
	for(i=0; i< 12; i++)
         {
		putc(CONSOLE, ch);
		putc(CONSOLE, '\n');
         }
        sleep(5);
        xexecl(prB, 'B');
}

/*------------------------------------------------------------------------
 *  prB  --  repeatedly print 'B' without ever terminating
 *------------------------------------------------------------------------
 */
void prB(int ch)
{
  int i;
	for(i=0; i< 12*80; i++)
		putc(CONSOLE, ch);
}
