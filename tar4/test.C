#include <conf.h>
#include <kernel.h>
#include "newSys.h"
#include "xfork.h"
/*------------------------------------------------------------------------
 *  xmain  
 *------------------------------------------------------------------------
 */

xmain()
{
    int checkXfork(),printMSG(),checkWait(),checkWaitAll(),checkWaitMul(),checkTmpMem(),checkfreep();
	
	//resume( create(checkWait, INITSTK, INITPRIO, "proc 1", 0) );
	//resume( create(checkWaitMul, INITSTK, INITPRIO, "proc 2", 0) );
	//resume( create(checkWaitAll, INITSTK, INITPRIO, "proc 3", 0) );
	//resume( create(checkXfork, INITSTK, INITPRIO, "proc 4", 0) );
	//resume( create(checkTmpMem, INITSTK, INITPRIO, "proc 5", 0) );
	resume( create(checkfreep, INITSTK, INITPRIO, "proc 6", 0) );
}

checkTmpMem(){
	int* arr;
	int i;
	printf("testing \"getmemForTime\"\n\n");
	printf("getting memory for 5 secs\n");
	getmemForTime(5*sizeof(int),5,&arr);
	printf("memory location = %x\n",arr);
	for(i = 0; i<6;i++){
		sleep(1);
		printf("sleep 1sec no %d\n",i);
	}
	printf("memory location = %x\n",arr);
	arr = getmem(5*sizeof(int));
	printf("mew memory location = %x\n",arr);
	freemem(arr,5*sizeof(int));
}

checkfreep(){
	int* arr;
	printf("testing \"freep\"\n\n");
	printf("getting memory\n");
	arr = getmem(5*sizeof(int));
	printf("memory location = %x\n",arr);
	freep(arr);
	printf("memory freed\n");
	arr = getmem(5*sizeof(int));
	printf("mew memory location = %x\n",arr);
	freemem(arr,5*sizeof(int));
}


checkXfork(){
	printf("testing \"xforkSonFirst\"\n\n");
	if(xforkSonFirst() == 0){
		printMSG("son\n",3);
	}else{
		printMSG("father\n",3);
	}
	
}

checkWait(){
	printf("testing \"wait\" with 1 son\n\n");
	if(xfork() == 0){
		printMSG("son\n",3);
	}else{
		xwait();
		printMSG("father\n",3);
	}
	return;
	
}

checkWaitMul(){	
	printf("testing \"wait\" with 3 son\n\n");
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
	printf("testing \"waitAll\" with 3 son\n\n");
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

