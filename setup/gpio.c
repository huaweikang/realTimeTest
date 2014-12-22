/***************************************************
 *
 *	Base on PKUnity86-2 HNC817
 *
 *
 ***************************************************
 *
 * File		:	gpio.c
 * Author	:	kang
 *
 ***************************************************
 */

#include <gpio.h>
#include <vmLib.h>
#include <stdio.h>

/***************************************************
 *
 *
 * 	Defines
 *
 *
 ***************************************************
*/

/******* Addresses of gpio ************************/

#define	GPIO_BASE_PHYS_ADDR	0xee500000
#define	GPIO_BASE_VIRT_ADDR	0xee500000
#define	GPIO_GPLR_OFFS		0x0
#define	GPIO_GPDR_OFFS		0x4
#define	GPIO_GPSR_OFFS		0x8
#define	GPIO_GPCR_OFFS		0XC

#define	GPIO_GPLR		(*(volatile unsigned int*) (GPIO_BASE_VIRT_ADDR + GPIO_GPLR_OFFS))
#define	GPIO_GPDR		(*(volatile unsigned int*) (GPIO_BASE_VIRT_ADDR + GPIO_GPDR_OFFS))
#define	GPIO_GPSR		(*(volatile unsigned int*) (GPIO_BASE_VIRT_ADDR + GPIO_GPSR_OFFS))
#define	GPIO_GPCR		(*(volatile unsigned int*) (GPIO_BASE_VIRT_ADDR + GPIO_GPCR_OFFS))

#define	GPIO3_MASK		0x00000008


/***************************************************
 *
 * 	Global Function
 *
 ***************************************************
 */

/*************gpioInit()***************************/
void gpioInit(void){
	int pageSize;
	pageSize = vmPageSizeGet();
	if(vmMap(NULL, GPIO_BASE_VIRT_ADDR, GPIO_BASE_PHYS_ADDR, pageSize) == ERROR){	//remap physcial address to virtual address
		printf("Error mapping memory errno\n");
	}
	GPIO_GPDR |= GPIO3_MASK;		//switch gpio3 to output mode
}

/***********gpioSet()*****************************/
void gpioSet(void){
	GPIO_GPSR = GPIO3_MASK;			//Switch on gpio 3 
}


/*********gpioClr()*****************************/
void gpioClr(void){
	GPIO_GPCR = GPIO3_MASK;		 	//Switch off gpio 3
}
