#include <kernel.h>
#include <proc.h>
#include "wait.h"

SYSCALL xwait(){
	if(findNumOfSons(currpid)==0)
		return(SYSERR);
	wait_cnt[currpid] = 1;
	suspend(currpid);
	return(OK);
}


SYSCALL xwaitAll(){
	int sons = findNumOfSons(currpid);
	if(sons==0)
		return(SYSERR);
	wait_cnt[currpid] = sons;
	suspend(currpid);
	return(OK);
}

int findNumOfSons(int pid){
	int i;
	int cnt = 0;
	for(i=0 ; i<NPROC;i++){
		if(father[i]==pid)cnt++;
	}
	return cnt;
	
}