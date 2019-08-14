#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stm32f10x.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//This program is for learning use only and may not be used for any other purpose without the permission of the author.
//Mini STM32 development board
//Use SysTick's normal counting mode to manage the delay
//Include delay_us, delay_ms
//Positive point atom @ALIENTEK
//Technology Forum: www.openedv.com
//Modified date: 2011/5/11
//Version: V1.4
//Copyright, piracy will be investigated.
//Copyright(C) Punctual atom 2009-2019
//All rights reserved
//********************************************************************************
//V1.2 modification instructions
// Fixed an error in the call to the end of the loop
// To prevent delay inaccuracy, using do while structure!

//V1.3 modification instructions
// Added support for UCOSII latency.
// If ucosII is used, delay_init will automatically set the value of SYSTICK to correspond to the TICKS_PER_SEC of ucos.
//delay_ms and delay_us have also been modified for ucos.
//delay_us can be used under ucos, and the accuracy is very high, more importantly, it does not take up extra timers.
//delay_ms can be used as OSTimeDly under ucos. When ucos is not started, it is implemented by delay_us, which is an accurate delay.
// Can be used to initialize the peripheral, after starting ucos delay_ms according to the length of the delay, choose OSTimeDly implementation or delay_us implementation.

//V1.4 modification description 20110929
// Modified the use of ucos, but when ucos is not started, the delay_ms interrupt can not respond to the bug.
////////////////////////////////////////////////////////////////////////////////// 	 
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























