#ifndef _MYAPP_CFG_H_
#define _MYAPP_CFG_H_
#include <ucos_ii.h>
//Set the task stack size
#define LED_STK_SIZE     64
#define LED1_STK_SIZE    64
#define LCD_STK_SIZE     64
#define KEY_STK_SIZE     64
#define KT_STK_SIZE   	 64
#define START_STK_SIZE   512

//Set task priority
#define LED_TASK_Prio       6
#define LED1_TASK_Prio      7
#define LCD_TASK_Prio       9
#define KEY_TASK_Prio       2
#define KT_TASK_Prio        4

#define START_TASK_Prio     10

//Task stack
OS_STK  TASK_LED1_STK[LED_STK_SIZE];
OS_STK  TASK_LED_STK[LED1_STK_SIZE];
OS_STK  TASK_LCD_STK[LCD_STK_SIZE];
OS_STK  TASK_START_STK[START_STK_SIZE];
OS_STK  TASK_KEY_STK[KEY_STK_SIZE];
OS_STK  TASK_KT_STK[KT_STK_SIZE];

//Mission statement
void TaskStart(void *pdata);
void TaskLed(void *pdata);
void TaskLed1(void *pdata);
void TaskLCD(void *pdata);
void TaskKey(void *pdata);
void KeepTime(void *pdata);
#endif  /* #ifndef _MYAPP_CFG_H_ */
