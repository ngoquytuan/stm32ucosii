#include "delay.h"
#include "sys.h"
#include "os_cpu.h"
#include "ucos_ii.h"
// If you use ucos, include the following header file. If not used, comment out.
//////////////////////////////////////////////////////////////////////////////////	 
//This program is for learning use only and may not be used for any other purpose without the permission of the author.
//Mini STM32 development board
// Use SysTick's normal counting mode to manage the delay
//Include delay_us, delay_ms
//Positive point atom @ALIENTEK
//Technology Forum: www.openedv.com
// Modified date: 2011/5/11
//Version: V1.4
//Copyright, piracy will be investigated.
//Copyright(C) punctual atom 2009-2019
//All rights reserved
//************************************************ ********************************
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

static u8  fac_us=0;//us Time delay multiplier			   
static u16 fac_ms=0;//Ms delay multiplier, under ucos, represents the number of ms per beat

//#ifdef OS_CRITICAL_METHOD 	//If OS_CRITICAL_METHOD is defined, it means using ucosII.
////systick Interrupt service function, used when using ucos
//void SysTick_Handler(void)
//{				   
//	OSIntEnter();		//Entry break
//    OSTimeTick();       //Calling ucos clock service program             
//    OSIntExit();        //Trigger task switching soft interrupt
//}
//#endif


// Initialize the delay function
// When using ucos, this function will initialize the clock beat of ucos
//SYSTICK clock is fixed at 1/8 of the HCLK clock
//SYSCLK: system clock
void delay_init(u8 SYSCLK)
{
#ifdef OS_CRITICAL_METHOD 	//If OS_CRITICAL_METHOD is defined, it means using ucosII.
	u32 reload;
#endif
 	SysTick->CTRL&=~(1<<2);	//SYSTICK uses an external clock source
	fac_us=SYSCLK/8;		//Fac_us needs to be used whether or not ucos is used
	    
#ifdef OS_TICKS_PER_SEC	 	//If the number of clock beats is defined, it means to use ucosII.
	reload=SYSCLK/8;		//The number of counts per second is in K  
	reload*=1000000/OS_TICKS_PER_SEC;//Set overflow time according to OS_TICKS_PER_SEC
							//Reload is a 24-bit register, the maximum value is 16777216, at 72M, about 1.86s
	fac_ms=1000/OS_TICKS_PER_SEC;//Represents the minimum unit that ucos can delay  
	SysTick->CTRL|=1<<1;   	//Turn on SYSTICK interrupt
	SysTick->LOAD=reload; 	//Interrupted every 1/OS_TICKS_PER_SEC seconds
	SysTick->CTRL|=1<<0;   	//Open SYSTICK 
#else
	fac_ms=(u16)fac_us*1000;//Under non-ucos, the number of systick clocks required for each ms 
#endif
}								    

#ifdef OS_TICKS_PER_SEC	//Ucos used
//delay nus
//nus is the number of us to delay.    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;	//LOAD value    	 
	ticks=nus*fac_us; 			//Number of beats required	  		 
	tcnt=0;
	told=SysTick->VAL;        	//Counter value just entered
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;//Note here that SYSTICK is a decrementing counter.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;//Time exceeds / equals the time to delay, then exits.
		}  
	}; 									    
}
//delay nms
//nms: the number of ms to delay
void delay_ms(u16 nms)
{	
	if(OSRunning==OS_TRUE)//If os is already running	    
	{		  
		if(nms>=fac_ms)//The delay time is greater than the minimum time period of ucos 
		{
   			OSTimeDly(nms/fac_ms);//Ucos delay
		}
		nms%=fac_ms;				//Ucos has been unable to provide such a small delay, using the normal way delay    
	}
	delay_us((u32)(nms*1000));	//Normal mode delay, ucos cannot start scheduling at this time.
}
#else
// Without ucos
//delay nus
//nus is the number of us to delay.	    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //Time loading	  		 
	SysTick->VAL=0x00;        //Empty counter
	SysTick->CTRL=0x01 ;      //Start counting down 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//Waiting time to arrive   
	SysTick->CTRL=0x00;       //Close counter
	SysTick->VAL =0X00;       //Empty counter	 
}
//delay nms
// Note the scope of nms
//SysTick->LOAD is a 24-bit register, so the maximum delay is:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK unit is Hz, nms is in ms
//For 72M conditions, nms<=1864
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//Time loading (SysTick->LOAD is 24bit)
	SysTick->VAL =0x00;           //Empty counter
	SysTick->CTRL=0x01 ;          //Start counting down  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//Waiting time to arrive   
	SysTick->CTRL=0x00;       //Close counter
	SysTick->VAL =0X00;       //Empty counter	  	    
} 
#endif
			 



































