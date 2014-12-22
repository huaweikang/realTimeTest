#include <stdio.h>
#include <taskLib.h>
#include <string.h>

#define	DUMMY_PRIORITY	101
#define	LOW_PRIORITY	100
#define	HIGH_PRIORITY	99
#define	STACK_SIZE		2048
#define DUMMY_NUM		128
#define	DUMMY_STR_LEN		30

unsigned int TscMLTCOneStart, TscMLTCOneEnd;
int taskMLTCOneA, taskMLTCOneB, taskMLTCOneDummy[DUMMY_NUM];

void TaskMLTCOneDummy(void){
	//can not sure what context should be
	while(1){
	}

}

void TaskMLTCOneA(void){
	while(1){
		//get start time
		TscMLTCOneStart = pentiumTscGet32();
		//resume task B
		taskResume(taskMLTCOneB);
		//get end time
		TscMLTCOneEnd	= pentiumTscGet32();

		//output max load task context test one time via console
		printf("%u\n", TscMLTCOneEnd - TscMLTCOneStart);
	}
}

void TaskMLTCOneB(void){
	while(1){
		taskSuspend(taskMLTCOneB);
		taskSuspend(taskMLTCOneB);
	}
}

//Genenate name for dummy task
void GenenateName(char *str, int num){
	int len, j;
	strcpy(str, "taskMLTCOneDummy");
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

int maxLoadTaskContextTestOne(void){
	int i;
	char str[DUMMY_STR_LEN];

	//spawn task dummy 0-127
	for(i = 0; i < DUMMY_NUM; i++){
		GenenateName(str, i);
		if((taskMLTCOneDummy[i] = taskSpawn(str, DUMMY_PRIORITY, NULL, STACK_SIZE, (FUNCPTR)TaskMLTCOneDummy, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR){
			printf("Spawn %d dummy task failed\n", i);
			return -1;
		}
	}

	//create task A and task B
	if((taskMLTCOneA = taskCreate("taskMLTCOneA", LOW_PRIORITY, NULL, STACK_SIZE, (FUNCPTR)TaskMLTCOneA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR){
		printf("Create Task A failed\n");
		return -1;
	}
	if((taskMLTCOneB = taskCreate("taskMLTCOneB", HIGH_PRIORITY, NULL, STACK_SIZE, (FUNCPTR)TaskMLTCOneB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR){
		printf("Create Task B failed\n");
		return -1;
	}


	//resume task B
	taskResume(taskMLTCOneB);
	//resume task A
	taskResume(taskMLTCOneA);

	printf("It is max load task context Test 1\n");
	return 0;
}
