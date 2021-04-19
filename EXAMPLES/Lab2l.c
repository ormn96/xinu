#include <conf.h>
#include <kernel.h>

unsigned long int NUM;
int N,WinnerPid,WinnerStart,divider_of_NUM;
int Not_prime_Flag=0;
int deathSem;
xmain()
{
	int start_search,difference,i;
	int is_Prime();
	int sem;
	sem = screate(1);
	deathSem = screate(0);
	printf("Enter unsigned long integer NUM:\n");
	scanf("%lu",&NUM);
	printf("Enter number of processes:\n");
	scanf("%d",&N);
	if(NUM%2 == 0 && NUM != 2){
		printf("The number is not prime(divided by 2)\n");
		return;
	}
	else{
		//wait(sem);
		for( i = 0;i < N;i++){
			start_search = 3 + 2 * i; //ערך התחלתי
			difference= 2 * N; //הפרשים
			resume(create(is_Prime,INITSTK, INITPRIO, "prime", 3, start_search, difference, sem));
		}
	}
	for( i = 0;i < N;i++){
		wait(deathSem);
	}
	if(Not_prime_Flag){
		printf("%ld is NOT prime\n",NUM);
		printf("%ld / %d = %d\n",NUM,divider_of_NUM,(NUM/divider_of_NUM));
		printf("Winner pid= %d Winner start= %d\n",WinnerPid,WinnerStart);
	}
	else{
		printf("%ld is prime number\n",NUM);
	}
	//signal(sem);
	return;
}

is_Prime(start_search,difference,sem)
int start_search, difference, sem;
{
   int curr_pid = getpid();
   unsigned long int i;
   for( i = start_search;i < (NUM/2); i += difference){
	   wait(sem);
	   if(Not_prime_Flag){
			printf("Process with pid %d started with %d incremented by %d. Tested up to %d\n", curr_pid, start_search, difference, i);
			signal(sem);
			signal(deathSem);
			kill(curr_pid);
	   }
	   if(NUM % i == 0){
		   Not_prime_Flag=1;
		   divider_of_NUM=i;
		   WinnerPid=curr_pid;
		   WinnerStart=start_search;
	   }
	   signal(sem);
   }
   signal(deathSem);
   return;
}
