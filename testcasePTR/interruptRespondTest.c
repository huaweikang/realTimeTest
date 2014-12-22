#include <stdio.h>
#include <sigLib.h>
#include <taskLib.h>
#include <logLib.h>

#define LOW_PRIORITY	100
#define	HIGH_PRIORITY	99
#define	STACK_SIZE		2048

unsigned int TscIRStart, TscIREnd;
int taskIRA, taskIRMaster;

void IRAction(int signal){
	TscIREnd = pentiumTscGet32();
}

void TaskIRA(void){
	
	while(1){
		TscIRStart = pentiumTscGet32();
		
		if(kill(taskIRMaster, SIGINT) == ERROR)
				printf("send signal failed\n");
		
		//output the interrupt respond test time to console
		printf("%u\n", TscIREnd - TscIRStart);
	}
}
 
int interruptRespondTest(void){
	//init a sigal
	struct sigaction newAction;
	newAction.sa_handler = IRAction;	//set the new handler
	sigemptyset(&newAction.sa_mask);	//no other signals blocked
	newAction.sa_flags = 0;			//no special options

	if(sigaction(SIGINT, &newAction, NULL) == -1){
		printf("Could not install signal handler\n");
	}
	
	//create task A
	if((taskIRA = taskCreate("taskIRA", LOW_PRIORITY, NULL, STACK_SIZE, (FUNCPTR)TaskIRA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == NULL){
		printf("Could not create task A\n");
		return -1;
	}
	taskIRMaster = taskIdSelf();
	
	//resume task A
	taskResume(taskIRA);
	
	printf("It is interrupt respond test.\n");
	return 0;
}
