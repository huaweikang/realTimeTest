#include <stdio.h>
#include <vxWorks.h>
#include <taskLib.h>
#include <msgQLib.h>

#define	LOW_PRIORITY	100
#define	HIGH_PRIORITY	99
#define	STACK_SIZE 2048
#define	MsgQLen	1
#define	MaxMsgLen 20

int taskMTA, taskMTB;
unsigned int MTTscStart, MTTscEnd;
MSG_Q_ID msgQId;

void TaskMTA(void){
	char buffer[MaxMsgLen];
	while(1){
		//get TSC time
		MTTscStart = pentiumTscGet32();
		//send message
		msgQSend(msgQId, buffer, sizeof(buffer),  WAIT_FOREVER, 0);
	}
}

void TaskMTB(void){
	char buffer[MaxMsgLen];
	while(1){
		
		msgQReceive(msgQId, buffer, sizeof(buffer),  WAIT_FOREVER);
		//get TSC time
		MTTscEnd = pentiumTscGet32();
		//send message
		//output message translate time, now output to console with printf function
		
		//printf("start is %u, end is %u\n", MTTscStart, MTTscEnd);
		printf("%u\n", MTTscEnd - MTTscStart);
	}
}

int messageTranslateTest(void){
	//create message queue	
	if((msgQId = msgQCreate(MsgQLen, MaxMsgLen, MSG_Q_FIFO)) == NULL){
		printf("Could not create message queue\n");
		return -1;
	}
	//create task A and B
	if((taskMTA = taskCreate("taskMTA", LOW_PRIORITY, NULL, STACK_SIZE, (FUNCPTR)TaskMTA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == NULL){
		printf("Could not create task A\n");
		return -1;
	}
	if((taskMTB = taskCreate("taskMTB", HIGH_PRIORITY, NULL, STACK_SIZE, (FUNCPTR)TaskMTB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == NULL){
		printf("Could not create task B\n");
		return -1;
	}

	//reset Tsc
	pentiumTscReset();
	//resume task B
	taskResume(taskMTB);
	//resume task A
	taskResume(taskMTA);
	
	printf("It is message translate test.\n");
	return 0;
}
