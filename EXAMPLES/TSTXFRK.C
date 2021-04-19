 /* tstxfrk.c - test xfork */

#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <q.h>
#include <bios.h>
#include <kbdio.h>



/*------------------------------------------------------------------------
 *  xmain  --  example of creating processes in PC-Xinu
 *------------------------------------------------------------------------
 */

void process()
{
  int n = 100;

  int id, *nptr;

  nptr = &n;
  if ( ( id =  xfork() ) == 0 )
  {  /* select child process */
     printf("\n**************  child process ***********\n");

     *nptr = 999;  /* Only this line is different */

     printf("PID is %d and ID is %d.\n", getpid(), id);
     printf("n is %d, *nptr is %d and nptr is %d.\n", n, *nptr, nptr);
     printf("\n**************  child process ***********\n");

     sleep(6);
     printf("\n Press enter to continue ... ");
     getchar();

     printf("\n**************  child process ***********\n");
     printf("PID is %d and ID is %d.\n", getpid(), id);
     printf("n is %d, *nptr is %d and nptr is %d.\n", n, *nptr, nptr);
     printf("\n**************  child process ***********\n");

     n = 707;

     printf("\n**************  child process ***********\n");
     printf("PID is %d and ID is %d.\n", getpid(), id);
     printf("n is %d, *nptr is %d and nptr is %d.\n", n, *nptr, nptr);
     printf("\n**************  child process ***********\n");

     return;
  }


     sleep(5);
     printf("\n**************  parent process ***********\n");
     printf("PID is %d and ID is %d.\n", getpid(), id);
     printf("n is %d, *nptr is %d and nptr is %d.\n", n, *nptr, nptr);
     printf("\n**************  parent process ***********\n");

     n = 200;
     *nptr = 300;
     printf("\n**************  parent process ***********\n");
     printf("PID is %d and ID is %d.\n", getpid(), id);
     printf("n is %d, *nptr is %d and nptr is %d.\n", n, *nptr, nptr);
     printf("\n**************  parent process ***********\n");
     while(1)
       ;

} /* process */

xmain()
{
resume(create(process, INITSTK, INITPRIO, "process",0));
} /* xmain */

