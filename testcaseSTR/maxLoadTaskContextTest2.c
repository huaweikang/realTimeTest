#include <stdio.h>
#include <taskLib.h>
#include <string.h>
#include <vxWorks.h>

#define	DUMMY_PRIORITY	101
#define	LOW_PRIORITY	100
#define	HIGH_PRIORITY	99
#define	STACK_SIZE		2048
#define DUMMY_NUM		128
#define	DUMMY_STR_LEN		30

unsigned int TscMLTCTwoStart, TscMLTCTwoEnd;
int taskMLTCTwoA, taskMLTCTwoB, taskMLTCTwoDummy[DUMMY_NUM];

void TaskMLTCTwoDummy(void){
	//can not sure what context should be
	while(1){
	}

}

void TaskMLTCTwoA(void){
	while(1){

	}
}

void TaskMLTCTwoB(void){
	while(1){
		//get start time
		TscMLTCTwoStart = pentiumTscGet32();

		taskSuspend(taskMLTCTwoA);
		
		
		taskResume(taskMLTCTwoA);
		
		//get end time
		TscMLTCTwoEnd = pentiumTscGet32();

		//output max load task context test two time via console
		printf("%u\n", TscMLTCTwoEnd - TscMLTCTwoStart);
	}
}

//Genenate name for dummy task
void GenenateName(char *str, int num){
	int len, j;
	strcpy(str, "taskMLTCTwoDummy");
	len = strlen(str);
	j = DUMMY_STR_LEN;
	do{
		str[--j] = '0' + num%10;
		num = num/10;
	}while(num);
	
	while(j < DUMMY_STR_LEN){
		str[len++] = str[j++];
	}
	str[len] = '\0';
}

int maxLoadTaskContextTestTwo(void){
	int i;
	char str[DUMMY_STR_LEN];

	//spawn task dummy 0-127
	for(i = 0; i < DUMMY_NUM; i++){
		GenenateName(str, i);
		if((taskMLTCTwoDummy[i] = taskSpawn(str, DUMMY_PRIORITY, NULL, STACK_SIZE, (FUNCPTR)TaskMLTCTwoDummy, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR){
			printf("Spawn %d dummy task failed\n", i);
			return -1;
		}
	}

	//create task A and task B
	if((taskMLTCTwoA = taskCreate("taskMLTCTwoA", LOW_PRIORITY, NULL, STACK_SIZE, (FUNCPTR)TaskMLTCTwoA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR){
		printf("Create Task A failed\n");
		return -1;
	}
	if((taskMLTCTwoB = taskCreate("taskMLTCTwoB", HIGH_PRIORITY, NULL, STACK_SIZE, (FUNCPTR)TaskMLTCTwoB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR){
		printf("Create Task B failed\n");
		return -1;
	}


	//rest Time Stamp Counter
	pentiumTscReset();
	
	//resume task A
	taskResume(taskMLTCTwoA);

	//resume task B
	taskResume(taskMLTCTwoB);

	printf("It is max load context test 2\n");
	return 0;
}
