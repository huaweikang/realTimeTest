#include <stdio.h>
#include <vxWorks.h>
#include <taskLib.h>

#define STACK_SIZE 2048
#define TASK_NUM	10
#define	TASK_NAME_LEN	20


int taskCSTOneMTA, tNum;
int task[TASK_NUM];

unsigned int TscCSTOneMTStart, TscCSTOneMTEnd;

void TaskCSTOneMTA(void)	//low priority
{
	int i;
	while(1){
		//get time1
		for(i = 0; i < TASK_NUM; i++){
				tNum = i;
				
				TscCSTOneMTStart = pentiumTscGet32();
					
				//resume Task tNum
				taskResume(task[tNum]);
						
				//get time2
				TscCSTOneMTEnd = pentiumTscGet32();
					
				//Caculate the time gap between Context Switch
				printf("the Context Switch time Between A and %d task is %d\n", tNum, TscCSTOneMTEnd - TscCSTOneMTStart);
		}
		
	}
	
}

void TaskCSTOneMTB(void)	//high priority
{
	while(1){
		printf("------------------------------------------------------\n");
		//suspend self
		printf("tNum is %d\n", tNum);
		
		taskSuspend(task[tNum]);
		//suspend self
		taskSuspend(task[tNum]);
	}
}


int Tmain(){
	int i;
	char taskName[10] = "task";
	taskName[5] = '\0';
	
	//creat taskCSTOneMTA
	taskCSTOneMTA = taskCreate("taskCSTOneMTA", 100, 0, STACK_SIZE, (FUNCPTR)TaskCSTOneMTA, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0);
	//creat task array
	
	for(i = 0; i < TASK_NUM; i++){
		taskName[4] = '0' + i;
		task[i] = taskCreate(taskName, 99, 0, STACK_SIZE, (FUNCPTR)TaskCSTOneMTB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}
		 
	
	
	//rest Time Stamp Counter
	pentiumTscReset();

	taskResume(taskCSTOneMTA);
	
	return 0;
}
