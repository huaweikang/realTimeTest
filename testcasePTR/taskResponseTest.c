#include <stdio.h>
#include <sigLib.h>
#include <semLib.h>
#include <taskLib.h>

#define STACK_SIZE	2048


int taskRTA, taskRTB, taskRTMaster;
int flag;
unsigned int TscStart, TscEnd;
SEM_ID semMutex;

void Action(int signal)
{
	if(flag == 1){
		semGive(semMutex);
	}
}

void TaskRTA(void){
	while(1){
		flag = 1;
		TscStart = pentiumTscGet32();
		kill(taskRTMaster, SIGINT);
	}
	
}
void TaskRTB(void){
	while(1){
		semTake(semMutex, WAIT_FOREVER); 
		TscEnd	= pentiumTscGet32();
		flag = 0;
		printf("%u\n", TscEnd - TscStart);
	}
	
}

int taskResponseTest(void)
{
	struct sigaction newAction;
	//init signal
	newAction.sa_handler = Action;		//set the new handler
	sigemptyset(&newAction.sa_mask);	//no other signals blocked
	newAction.sa_flags = 0;	//no special options

	if(sigaction(SIGINT, &newAction, NULL) == -1)
		printf("Could not install signal handler\n");

	//init semaphore
	semMutex = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
	

	//create task A and B
	if((taskRTA = taskCreate("taskRTA", 100, NULL, STACK_SIZE, (FUNCPTR)TaskRTA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == NULL)
		printf("Could not create task A\n");
	if((taskRTB = taskCreate("taskRTB", 99, NULL, STACK_SIZE, (FUNCPTR)TaskRTB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == NULL )
		printf("Could not create task B\n");
	
	taskRTMaster = taskIdSelf();
	
	//resume task B
	taskResume(taskRTB);
	//resume task A
	taskResume(taskRTA);

	printf("It is task response test.\n");
	return 0;
}
