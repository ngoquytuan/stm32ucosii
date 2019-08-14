#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//Mini STM32 development board
//LED driver code
//Positive point atom @ALIENTEK
//2012/2/27

//LED port definition
#define LED0 PAout(1)// PA1
#define LED1 PDout(2)// PD2	

void LED_Init(void);//initialization

		 				    
#endif
