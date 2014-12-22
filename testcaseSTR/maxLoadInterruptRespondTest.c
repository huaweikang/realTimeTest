#include <stdio.h>
#include <taskLib.h>
#include <logLib.h>
#include <intLib.h>
#include <iv.h>
#include <wdLib.h>
#include <sysLib.h>

#define LOW_PRIORITY	100
#define	STACK_SIZE		2048
#define	GPIO_INT_NUM	5

int count;
int taskMLIR;
WDOG_ID	Wdid;

void irq_handler(void){
	count++;
}
void timer_irq(void){
	printf("count=%d\n", count);
	wdCancel(Wdid);
}

void maxLoadInterruptEntry(void){
	while(1){
	
	}
}
int maxLoadInterruptRespondTest(void){
	int lockKey;
	
	if((taskMLIR = taskSpawn("taskMLIR", LOW_PRIORITY, NULL, STACK_SIZE, (FUNCPTR)maxLoadInterruptEntry, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR ){
		printf("Can not creat task A\n");
		return -1;
	}
	intConnect(INUM_TO_IVEC(GPIO_INT_NUM), irq_handler, 0);
	Wdid = wdCreate();
	wdStart(Wdid, sysClkRateGet(), (FUNCPTR)timer_irq, 0);
	lockKey = intLock();
	count = 0;
	intUnlock(lockKey);
	
	return 0;
}
