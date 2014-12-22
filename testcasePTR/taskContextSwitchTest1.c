#include <stdio.h>
#include <vxWorks.h>
#include <taskLib.h>
#include <gpio.h>

#define STACK_SIZE 2048
#define LOW_PRIORITY	100
#define	HIGH_PRIORITY	99

int taskCSTOneA, taskCSTOneB;
unsigned int TscCSTOneStart, TscCSTOneEnd;

void TaskCSTOneA(void)	//low priority
{
	//int k = TEST_TIME;
	while(1){
		
		//gpioClr();
		//gpioSet();
		//get time1
		TscCSTOneStart = pentiumTscGet32();
		//gpioClr();
		//resume Task B
		taskResume(taskCSTOneB);
			
		//get time2
		TscCSTOneEnd = pentiumTscGet32();
		//gpioSet();
		//Caculate the time gap between Context Switch
		printf("%u\t%u\t%u\n", TscCSTOneEnd, TscCSTOneStart, TscCSTOneEnd - TscCSTOneStart);
	}
	
}

void TaskCSTOneB(void)	//high priority
{
	while(1){
		//suspend self
		taskSuspend(taskCSTOneB);
		//suspend self
		taskSuspend(taskCSTOneB);
	}
}


int taskContextSwitchTestOne(){
	//creat taskCSTOneA
	taskCSTOneA = taskCreate("taskCSTOneA", LOW_PRIORITY, NULL, STACK_SIZE, (FUNCPTR)TaskCSTOneA, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0);
	//creat taskCSTOneB
	taskCSTOneB = taskCreate("taskCSTOneB", HIGH_PRIORITY, NULL, STACK_SIZE, (FUNCPTR)TaskCSTOneB, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0);
	
	//rest Time Stamp Counter
	pentiumTscReset();
	gpioInit();
	gpioSet();
	//Resume taskCSTOneB
	taskResume(taskCSTOneB);
	//Resume taskCSTOneA
	taskResume(taskCSTOneA);
	
	printf("It is task context switch test 1\n");
	
	return 0;
}
