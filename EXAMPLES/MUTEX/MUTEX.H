/* mutex.h */

#define MUTEX_VAR_INITIALIZER (MUTEX_REC_PTR)NULL

typedef struct mutex_rec
{
  int nsem;
  int pid;
} MUTEX_REC, *MUTEX_REC_PTR, MUTEX_REC_VAR, *MUTEX_VAR;

extern SYSCALL mutex_lock(MUTEX_REC_PTR *mutex_var);
extern SYSCALL mutex_unlock(MUTEX_REC_PTR *mutex_var);
