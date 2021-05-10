#include <kernel.h>
#include <mem.h>

SYSCALL freep(char*p){
	int size = *((word*)(p - sizeof(word)));
	freemem(p,size);
	return(OK);
}