#include <conf.h>
#include <kernel.h>
#include <stdio.h>

int	flag=1;
int N;	
int flagMutex, processDoneSem,printsem;
int winnerStart, winnerPID;
unsigned long int num , winnerLast;

xmain()
{
	int checkIfPrime(),i;
	//user input
	printf("Enter unsigned long integer NUM:\n");
	scanf("%lu",&num);
	printf("Enter number of processes:\n");
	scanf("%d",&N);
	
	//check even number
	if(num%2 == 0){
		if(num!=2){
			printf("%lu is NOT prime\n",num);
			printf("%lu / %d = %lu\n",num, 2 , num/2);
			printf("main found this number\n");
		}else{
			printf("2 is prime\n");
		}
		return 0;
	}
	
	//init semaphors and processes
	flagMutex = screate(1);
	processDoneSem = screate(0);
	printsem = screate(1);
	for(i=0;i<N;i++){
		resume(create(checkIfPrime,INITSTK,INITPRIO,"prod",1,i));
	}

	//wait for all processes to finish
	for(i=0;i<N;i++){
		wait(processDoneSem);
	}

	//check result and print appropriate message
	if(flag == 1){
		printf("%lu is prime\n",num);
	}else{
		printf("%lu is NOT prime\n",num);
		printf("%lu / %lu = %lu\n",num, winnerLast , num/winnerLast);
		printf("Winner pid = %d winner start = %d\n",winnerPID,winnerStart);
	}
	
	//clean semaphors
	sdelete(flagMutex);
	sdelete(processDoneSem);
	sdelete(printsem);
	return 0;
}

//check if the global argument "num" is prime number, initialzing worker with ID "index"
checkIfPrime(index)
int index;
{
	int startNum,i;
	unsigned long int cur;
	startNum =3+2*(index); //initial number
	for(cur = startNum ; cur<(num/2)&&flag==1;cur=cur + 2*N){
		wait(flagMutex);
		if(num%cur ==0 && flag){
			flag = 0; //set all processes to exit
			
			//save winner data
			winnerPID = getpid();
			winnerLast = cur;
			winnerStart = startNum;
		}
		signal(flagMutex);	
	}
	//remove the increment of last iteration if and only if the process already started the check
	if(cur!=startNum)
		cur=cur - 2*N;
	
	//print process message
	wait(printsem);
	printf("Process with pid %d started with %d incremented by %d. Tested up to %lu\n",getpid(),startNum,2*N,cur);
	signal(printsem);
	signal(processDoneSem);//signal done
	return 0;
}
