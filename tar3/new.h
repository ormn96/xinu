extern int pprio[NPROC];
extern int peffec[NPROC];
extern long int runnable_time[NPROC];
extern long int current_time[NPROC];

#define lastkeyeffec(tail)   peffec[q[(tail)].qprev]
