#include <conf.h>
#include <kernel.h>
#include <stdio.h>

int flag = 1;
int N;	
unsigned long int num ;
int flagSem,deathSem;
int printSem;

xmain()
{
	int checkIfPrime(),i;
	//user input
	printf("Enter unsigned long integer NUM:\n");
	scanf("%lu",&num);
	printf("Enter number of processes:\n");
	scanf("%d",&N);
	if(num%2==0){
		if(num!=2){
			printf("%lu is NOT prime\n",num);
			printf("%lu / 2 = %lu\n",num , num/2);
		}else{
			printf("2 is prime\n");
		}
	}
	flagSem = screate(1);
	deathSem = screate(0);
	printSem = screate(1);
	for(i=1;i<=N;i++){
		resume(create(checkIfPrime,INITSTK,INITPRIO,"prod",1,i));
	}
	
	return 0;
}

//check if the global argument "num" is prime number, initialzing worker with ID "index"
checkIfPrime(index)
int index;
{
	int i;
	unsigned long int start = 3+2*(index-1);
	unsigned long int cur = start;
	while(flag ==1 && cur<num/2){
		
		if(num%cur == 0 ){
			wait(printSem);
			printf("Process with pid %d started with %lu incremented by %d. Tested up to %lu\n",getpid(),start,2*N,cur);
			signal(printSem);
			
			signal(deathSem);
			
			wait(flagSem);
			if(flag==1)
				flag = 0;
			else{
				signal(flagSem);
				return 0;
			}
			signal(flagSem);
			
			for(i=0;i<N;i++){
				wait(deathSem);
			}
			
			wait(printSem);
			printf("%lu is NOT prime\n",num);
			printf("%lu / %lu = %lu\n",num, cur , num/cur);
			printf("Winner pid = %d winner start = %d\n",getpid(),start);
			signal(printSem);
			
			
			sdelete(flagSem);
			sdelete(deathSem);
			sdelete(printSem);
			return 0;
		}
		cur += 2*N;
	}
	wait(printSem);
	printf("Process with pid %d started with %lu incremented by %d. Tested up to %lu\n",getpid(),start,2*N,cur);
	signal(printSem);
	
	signal(deathSem);
	return 0;
}
