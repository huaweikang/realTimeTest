#include <stdio.h>
#include <taskLib.h>
#include <sigLib.h>
#include <logLib.h>

#define LOW_PRIORITY	100
#define	HIGH_PRIORITY	99
#define STACK_SIZE		2048

unsigned int TscIRRStart, TscIRREnd;
int taskIRRA, taskIRRB, taskIRRMaster;

void IRRAction(int signal){
	taskResume(taskIRRB);
	
	TscIRRStart = pentiumTscGet32();
}

void TaskIRRA(void){
	
	
	while(1){
		//printf("It is in task A\n");
		kill(taskIRRMaster, SIGINT);
	}
}

void TaskIRRB(void){
		
	while(1){
		taskSuspend(taskIRRB);
		TscIRREnd = pentiumTscGet32();
			
		//output the interrupt restore reshedule test time to console
		printf("%u\n", TscIRREnd - TscIRRStart);
	}
}

int interruptRestoreRescheduleTest(void){
	//init a sigal
		struct sigaction newAction;
		newAction.sa_handler = IRRAction;	//set the new handler
		sigemptyset(&newAction.sa_mask);	//no other signals blocked
		newAction.sa_flags = 0;			//no special options
					
		if(sigaction(SIGINT, &newAction, NULL) == -1){
			printf("Could not install signal handler\n");
		}
	
	//create task A
	if((taskIRRA = taskCreate("taskIRRA", LOW_PRIORITY, NULL, STACK_SIZE, (FUNCPTR)TaskIRRA, 0, 0, 0, 0, 0, 0, 0, 0, 0,0)) == NULL){
		printf("Could not create task A\n");
		return -1;
	}
	
	//create task B
	if((taskIRRB = taskCreate("taskIRRB", HIGH_PRIORITY, NULL, STACK_SIZE, (FUNCPTR)TaskIRRB, 0, 0, 0, 0, 0, 0, 0, 0, 0,0)) == NULL){
		printf("Could not create task A\n");
		return -1;
	}
	
	taskIRRMaster = taskIdSelf();
	
	//resume task B
	taskResume(taskIRRB);
	//resume task A
	taskResume(taskIRRA);

	
	printf("It is interrupt restore reschedule test\n");
	return 0;
}
