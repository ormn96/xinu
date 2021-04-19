/* memtest.c - test memory allocation policy, best vs first fit */

#include <dos.h>
#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <q.h>
#include <bios.h>
#include <kbdio.h>


void complete_memory_report()
{
	int     ps;
	int index = 0;
	struct  mblock  *p, *q, *leftover;


	for ( q=&memlist, p=q->mnext ;
		(char *)p != NULL ;
		q=p, p=p->mnext )
	 {
	      unsigned endpoint, nextpoint, size;
	      index++;
	      printf("\nBlock number %d, starting point = %u, size = %u\n",
		     index, p, p->mlen); 
	      if (p->mnext != NULL)
	      {
		nextpoint = (unsigned) p->mnext;
		endpoint = (unsigned) ( ((unsigned)p) + p->mlen);
		size = nextpoint - endpoint; 
		printf("\nGap: from %u to %u, size = %u\n", 
		       endpoint,  nextpoint, size   );
	      }
	      else
		 printf("\n Final address = %u\n", 
		       ((unsigned)p)+(p->mlen));
	 } /* for */

 } /* complete memory report */


 void xmain()
{  
    char       *p, *q;

    p = getmem(15000);
    freemem(p,4000);
    freemem(p+10000,2000);
    printf("\nFirst report:\n");
    complete_memory_report();
    printf("\nPress Enter to continue...");
    getchar();
    q = getmem(1500);
    printf("\nSecond report:\n");
    complete_memory_report();
    printf("\nPress Ctrl-Break to return to command prompt\n");

} /* xmain */



