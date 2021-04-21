#include <butler.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include "new.h"

int calc_peffec(int pid){
	int p;
	
	if(pid<4)
		return pprio[pid];
	
	if(runnable_time[pid] == 0){
		if(pprio[pid] > (BTLRPRIO-1) )
			return BTLRPRIO-1;
		return 1 + pprio[pid];
	}
	p=1+(pprio[pid]*(runnable_time[pid]-current_time[pid]))/runnable_time[pid];
	if(p<1)return 1;
	if(p> (BTLRPRIO-1) )return BTLRPRIO-1;
	return p;
}


void fixReady(){
	int cur;
	int maxP=0;
	int maxIndex = -1;
	cur = q[rdyhead].qnext;
	while (cur!= rdytail){
		if(peffec[cur]>maxP){
			maxP=peffec[cur];
			maxIndex=cur;
		}		
		cur = q[cur].qnext;
	}
	if(maxIndex!=-1 && maxIndex!=q[rdytail].qprev){
		//remove maxIndex from ready queue
		q[q[maxIndex].qprev].qnext = q[maxIndex].qnext;
		q[q[maxIndex].qnext].qprev = q[maxIndex].qprev;
		
		//add maxIndex to tail
		q[maxIndex].qnext=rdytail;
		q[maxIndex].qprev=q[rdytail].qprev;
		q[q[rdytail].qprev].qnext=maxIndex;
		q[rdytail].qprev=maxIndex;
	}
}