#ifndef __SYS_H
#define __SYS_H	
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//This program is for learning use only and may not be used for any other purpose without the permission of the author.
//ALIENTEK Mini STM32 development board
// System clock initialization
//Positive point atom @ALIENTEK
//Technology Forum: www.openedv.com
// Modified date: 2010/5/27
//Version: V1.4
//Copyright, piracy will be investigated.
//Copyright(C) punctual atom 2009-2019
//All rights reserved
//******************************************************************************** 
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO port address mapping
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO port operation, only for a single IO port!
//Make sure the value of n is less than 16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //OUT 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //IN 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //OUT 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //IN 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //OUT 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //IN 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //OUT 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //IN 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //OUT 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //IN

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //OUT 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //IN

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //OUT 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //IN



//void NVIC_Configuration(void);



#endif
