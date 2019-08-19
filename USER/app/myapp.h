#ifndef _MYAPP_CFG_H_
#define _MYAPP_CFG_H_

#include <ucos_ii.h>
#include "os_cpu.h"
#include <stdio.h>
#include <time.h>
#include "w5500init.h"
#include "led.h"
#include "key.h"
#include "dns.h"
#include "sntp.h"
//Set the task stack size
#define S256_STK_SIZE     256
#define MIN_STK_SIZE     64
#define LED_STK_SIZE     64
#define LED1_STK_SIZE    64
#define LCD_STK_SIZE     64
#define KEY_STK_SIZE     64
#define KT_STK_SIZE   	 64
#define START_STK_SIZE   1024

//Set task priority
#define LED_TASK_Prio       6
#define LED1_TASK_Prio      7
#define LCD_TASK_Prio       9
#define KEY_TASK_Prio       2
#define KT_TASK_Prio        4

#define START_TASK_Prio     10
#define TaskCheckLinkEthPrio						11
#define TaskGetTimeFromNTPServerPrio		12

//Mission statement
void TaskStart(void *pdata);
void TaskLed(void *pdata);
void TaskLed1(void *pdata);
void TaskLCD(void *pdata);
void TaskKey(void *pdata);
void KeepTime(void *pdata);
void TaskCheckLinkEth(void *pdata);
void TaskGetTimeFromNTPServer(void *pdata);

void starttasks(void);
#endif  /* #ifndef _MYAPP_CFG_H_ */
