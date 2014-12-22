#include <stdio.h>
#include <vxWorks.h>
#include <taskLib.h>

#define STACK_SIZE 2048
#define	LOW_PRIORITY	100
#define	HIGH_PRIORITY	99


int taskCSTTwoA, taskCSTTwoB;
unsigned int TscCSTTwoStart, TscCSTTwoEnd;

void TaskCSTTwoA(void)	//low priority
{
	while(1){
		
	}
	
}

void TaskCSTTwoB(void)	//high priority
{
	while(1){
		//get time1
		TscCSTTwoStart = pentiumTscGet32();
		
		//suspend task A
		taskSuspend(taskCSTTwoA);
		
		//
		//suspend task A
		taskResume(taskCSTTwoA);
		
		//get time2
		TscCSTTwoEnd = pentiumTscGet32();
		
		printf("%u\n", TscCSTTwoEnd - TscCSTTwoStart);
	}
}


int taskContextSwitchTestTwo(){
	//creat taskCSTTwoA
	taskCSTTwoA = taskCreate("taskCSTTwoA", LOW_PRIORITY, NULL, STACK_SIZE, (FUNCPTR)TaskCSTTwoA, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0);
	//creat taskCSTTwoB
	taskCSTTwoB = taskCreate("taskCSTTwoB", HIGH_PRIORITY, NULL, STACK_SIZE, (FUNCPTR)TaskCSTTwoB, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0);
	
	//rest Time Stamp Counter
	pentiumTscReset();

	//Resume task A
	taskResume(taskCSTTwoA);

	//Resume taskCSTTwoB
	taskResume(taskCSTTwoB);
	
	printf("It is task context switch test 2\n");
	
	return 0;
}
