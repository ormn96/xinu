/* ex9.c - xmain, Inc1, Inc2, Inc3, Pr */

#include <conf.h>
#include <kernel.h>
#include "newSys.h"
#include "xfork.h"
/*------------------------------------------------------------------------
 *  xmain  --  example of creating processes in PC-Xinu
 *------------------------------------------------------------------------
 */

unsigned long int count1 = 0, count2 = 0, count3 = 0, count4 = 0;

xmain()
{
    int Inc(), Pr(),checkXfork(),printMSG(),checkWait(),checkWaitAll(),checkWaitMul();
	
	//resume( create(checkWait, INITSTK, INITPRIO, "proc 1", 0) );
	//resume( create(checkWaitMul, INITSTK, INITPRIO, "proc 2", 0) );
	//resume( create(checkWaitAll, INITSTK, INITPRIO, "proc 3", 0) );
	resume( create(checkXfork, INITSTK, INITPRIO, "proc 4", 0) );

	// chprio(getpid(),800);
    // resume( create(Inc, INITSTK, INITPRIO+10, "proc 1", 1, &count1) );
    // resume( create(Inc, INITSTK, INITPRIO+10, "proc 2", 1, &count2) );
    // resume( create(Inc, INITSTK, INITPRIO+5, "proc 3", 1, &count3) );
    // resume( create(Inc, INITSTK, INITPRIO, "proc 4", 1, &count4) );
    // resume( create(Pr, INITSTK, INITPRIO + 100, "proc 6", 0) );
}

checkXfork(){
	if(xforkSonFirst() == 0){
		printMSG("son\n",3);
	}else{
		printMSG("father\n",3);
	}
	
}

checkWait(){
	
	if(xfork() == 0){
		printMSG("son\n",3);
	}else{
		xwait();
		printMSG("father\n",3);
	}
	return;
	
}

checkWaitMul(){	
	if(xfork() == 0){
		printMSG("son1\n",2);
	}else{
		if(xfork() == 0){
			printMSG("son2\n",5);
		}else{
			if(xfork() == 0){
				printMSG("son3\n",10);
			}else{
				xwait();
				printMSG("father\n",5);
			}
		}
	}
	return;
}

checkWaitAll(){	
	if(xfork() == 0){
		printMSG("son1\n",5);
	}else{
		if(xfork() == 0){
			printMSG("son2\n",5);
		}else{
			if(xfork() == 0){
				printMSG("son3\n",5);
			}else{
				xwaitAll();
				printMSG("father\n",2);
			}
		}
	}
	return;
}

/*------------------------------------------------------------------------
 *  printMSG  --  print message n times
 *------------------------------------------------------------------------
 */
printMSG(char* msg,int n){
	int i;
	for(i=0;i<n;i++){
		printf(msg);
		sleep(1);
	}
}



/*------------------------------------------------------------------------
 *  Inc  --  Increment counter via pointer
 *------------------------------------------------------------------------
 */
Inc(int ptr)
{
  unsigned long int *ptr1;

  ptr1 = (unsigned long int *)ptr;
  while (1)
     (*ptr1)++;

}  /* Inc */

Pr()
{
  char str[80];

  while(1)
  {
    sleep(3);
    sprintf(str, "count1 = %lu, count2 = %lu, count3 = %lu, count4 = %lu\n",
                       count1, count2, count3, count4);
    printf(str);
   } /* while */

} /* Pr */
