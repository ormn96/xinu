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

void prA(), prch123();

xmain()
{

	resume( create(prA, INITSTK, INITPRIO, "proc 1", 1,  'A') );
}

/* xexecl - emulate unix execl in xinu */ 
/* CHANGE */
xexecl(void (*pf)(), int n, ...)
/* END OF CHANGE */
{
struct pentry *pptr;
char *saddr;
int *sp1, *a;
int ps, i;
int dummy;

disable(ps);
pptr = &proctab[currpid];
pptr->phasmsg = 0;
sp1 = (int *)(pptr->pbase + pptr->plen);

/* CHANGE */

pptr->pargs = n;
a = n + 1 + &n;
for(i=0; i < n; i++)
  *(--sp1) = *(--a);

/* END OF CHANGE */


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
        xexecl(prch123, 3, 'B', 'C', 'D');
}

/*------------------------------------------------------------------------
 *  prch123  --  repeatedly print 'ch1ch2ch3' without ever terminating
 *------------------------------------------------------------------------
 */
void prch123(int ch1, int ch2, int ch3)
{
  int i;
	for(i=0; i< 12*80; i++)
         {
                putc(CONSOLE, ch1);
                putc(CONSOLE, ch2);
                putc(CONSOLE, ch3);

         } /* for */

} /* prch123 */

