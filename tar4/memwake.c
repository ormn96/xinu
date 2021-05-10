#include <kernel.h>
#include <sleep.h>
#include "tmpmem.h"

void tmpMem_new_wakeup()
{
        int pid;
        long temp;

        while ( tmpMemNonempty() && (tmpMemFirstkey()) <= 0 ) {
                pid = tmpMemGetfirst();
                
                if (qmem[pid].slpTime == 0){
					freemem(qmem[pid].ptr,qmem[pid].memSize);
					*(qmem[pid].iptr) = NULL;//for the user
					qmem[pid].iptr = NULL;//for signaling that the id is free
				}
                else
                  if ( qmem[pid].slpTime > TICSD )
                     {
                      tmpMemInsertd(pid, TICSN);
                      qmem[pid].slpTime -= TICSD;
                     } /* if */
                  else
                  {
                      temp =  (( (long)qmem[pid].slpTime )*TICSN)/TICSD;  
                      tmpMemInsertd(pid, temp);
                      qmem[pid].slpTime = 0;
                  } /* else */
	}
        if ( (tmpMemNotEmpty = tmpMemNonempty()) != 0 ) 
		tmpMemTop = &tmpMemFirstkey();
}

