#include <stdio.h>
#include <sigLib.h>
#include <taskLib.h>

#define STACK_SIZE	2048
#define	LOW_PRIORITY	100

unsigned int TscIRNRStart, TscIRNREnd;
int taskIRNRA, taskIRNRMaster;

void IRNRAction(int signal){
	TscIRNRStart = pentiumTscGet32();
}

void TaskIRNRA(void){
	while(1){
		kill(taskIRNRMaster, SIGINT);
		TscIRNREnd = pentiumTscGet32();
	
		//output the interrupt restore non reshedule test time to console
		printf("%u\n", TscIRNREnd - TscIRNRStart);
	}
}
int interruptRestoreNonRescheduleTest(void){
	//init a sigal
	struct sigaction newAction;
	newAction.sa_handler = IRNRAction;	//set the new handler
	sigemptyset(&newAction.sa_mask);	//no other signals blocked
	newAction.sa_flags = 0;			//no special options

	if(sigaction(SIGINT, &newAction, NULL) == -1){
		printf("Could not install signal handler\n");
	}
	//create task A
	if((taskIRNRA = taskCreate("taskIRNRA", LOW_PRIORITY, NULL, STACK_SIZE, (FUNCPTR)TaskIRNRA, 0, 0, 0, 0, 0, 0, 0, 0, 0,0)) == NULL){
		printf("Could not create task A\n");
		return -1;
	}
	
	taskIRNRMaster = taskIdSelf();
	
	//resume task A
	taskResume(taskIRNRA);
	
	printf("It is interrupt restore non reschedule test\n");
	
	return 0;
}
