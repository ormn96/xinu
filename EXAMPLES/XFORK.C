 /* xfork.c - xmain, prA, prB */

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


/* retip - compute ip of point of program */

int retip()
{
int ip1;

  asm {
        push ax
        mov ax,[BP+2]
        mov ip1,ax
        pop ax
      }
  return ip1;
}

/* xfork - xinu emulation of unix fork, will work
   only in the process main program, and pointers should not be used -
   pointers in the child process will point into the parent variable
   space */

int xfork()
{
char *saddr;
int *sp1, *sp2, *sp3, *sp4;
int ps, bp1;
int dummy;
int pid;
struct pentry *pptr, *pptr1;
int ip1;


disable(ps);
pptr = &proctab[currpid];
pid = create(pptr->paddr,pptr->plen, pptr->pprio, pptr->pname,0);

if (pid == SYSERR)
{
  restore(ps);
  return SYSERR;
} /* if */

pptr1 = &proctab[pid];

asm mov sp1,sp
sp2 = pptr->pbase + pptr->plen;
sp3 = pptr1->pbase + pptr1->plen;

/* give child process a duplicate stack */

for(;sp2 >= sp1;)
{
  *sp3 = *sp2;
  sp2--;
  sp3--;
}

/* compute instruction pointer for child process */


ip1 = retip();

/* child process  starts HERE */

if (currpid != pid) /* parent process only */
   {
	*(int *)sp3 = ip1;	/* simulate a context switch	*/
        sp3 -= 1;

        /* simulate call to ctxsw */

        /* bp adjusting - necessary because our xinu does not support
              virtual addressing, but rather uses real addressing */

        /* bp adjusting of ctxsw for child process - real mode */

        asm mov bp1,bp
        *(int *)sp3 = ((int)pptr1->pbase) + ((bp1 -((int)pptr->pbase)));
	sp3 -= 1;				/* 1 word for bp		*/
	*(int *)sp3 = INITF;		/* FLAGS value			*/
        sp3 -= 1;
	sp3 -= 1;			/* 2 words for si and di	*/

        /* complete emulation of ctxsw */

        pptr1->pregs = sp3;

        /* bp adjusting of xfork for child process - real mode */
        asm mov bp1,bp
        sp4 =(int *) ( ((int)pptr1->pbase) + (( bp1 -((int)pptr->pbase)) ));

        /* bp adjusting of xmain for child process - real mode */
        asm {
             push ax
             mov ax,[bp]
             mov bp1,ax
             pop ax
            }
        *sp4 = ( ((int)pptr1->pbase) + (( bp1 -((int)pptr->pbase)) ));

    resume(pid);
    restore(ps);
    return pid;  


   } /* if */
 else
  return 0;   /* child process only */

} /* xfork */


