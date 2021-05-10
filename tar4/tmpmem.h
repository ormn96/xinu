
#define NMEM 50  //50
#define NQMEM 50 + 2 //50 + head

struct qentmem {			/* one for each process plus two for    */
				/* each list                            */
	int   qkey;		/* key on which the queue is ordered    */
	int   qnext;		/* pointer to next process or tail      */
	int   qprev;		/* pointer to prev process or head      */
	int	  slpTime;
	int   memSize;
	void* ptr;
	void**   iptr;		/* pointer to previous process or head  */
};

extern  struct  qentmem qmem[];
extern  int tmpMemNotEmpty;
extern int tmpMemHead;
extern int tmpMemTail;
extern	int	*tmpMemTop;		/* address of first key on clockq	*/

#define tmpMemIsempty()   (qmem[tmpMemHead].qnext >= (NQMEM-2))
#define tmpMemNonempty()  (qmem[tmpMemHead].qnext < (NQMEM-2))
#define tmpMemFirstkey()  (qmem[qmem[tmpMemHead].qnext].qkey)


void tmpMem_new_wakeup();
int tmpMemDequeue(int item);
int	tmpMemGetfirst();
void tmpMemInsertd(int id,int key);

//wakeup free