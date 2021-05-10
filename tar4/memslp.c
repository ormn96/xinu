#include <kernel.h>
#include <sleep.h>
#include "tmpmem.h"

SYSCALL	tmpMemSleept(int id,int n)
{
	int	ps;

	if ( n<0 )
		return(SYSERR);
	if (n == 0)
		return(OK);
    disable(ps);
    tmpMemInsertd(id,n);
	tmpMemNotEmpty = TRUE;
	tmpMemTop = & qmem[qmem[tmpMemHead].qnext].qkey;
    restore(ps);
	return(OK);
}

SYSCALL getmemForTime(int nbytes,int secs,void** ptr)
{
	int	ps;
	int timeTmp;
	int tmpMemId;
	char* tmpMem;
	
	if ( secs<0 )
		return(SYSERR);
	if (secs == 0) {
		return(OK);
	}
	disable(ps);
	if((tmpMemId = tmpMemGetOpenId())==SYSERR){
		restore(ps);
		return(SYSERR);
	}
	
	if((tmpMem = getmem(nbytes) )== NULL){
		restore(ps);
		return(SYSERR);
	}	

	qmem[tmpMemId].iptr = ptr;
	qmem[tmpMemId].ptr = tmpMem;
	qmem[tmpMemId].memSize = nbytes;
	*ptr = tmpMem;
	if(secs>TICSD){
		qmem[tmpMemId].slpTime = secs - TICSD;
		tmpMemSleept( tmpMemId, TICSN );
	}
	else{
		qmem[tmpMemId].slpTime = 0;
		tmpMemSleept( tmpMemId,(int)( ((long)secs*(long)TICSN) / (long)TICSD ) );
	}
	restore(ps);	
	return(OK);
}

LOCAL tmpMemGetOpenId(){
	int i;
	for(i=0;i<NQMEM-2;i++){
		if(qmem[i].iptr == NULL){
			return i;
		}
	}
	return(SYSERR);
}

