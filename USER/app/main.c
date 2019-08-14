
//Mini STM32 development board sample code
//UCOS Lab 3: Semaphore Usage Test
//Positive point atom @ALIENTEK
//Technology Forum: www.openedv.com	
/*
UCOSII Experiment 3: UCOSII semaphore use test

1. Experimental purpose: semaphore creation request to send use test
2. Experimental phenomenon: press KEY0 button LED0 will flash 5 times (10 times reverse)
		Press KEY1 button LED0 will reverse once
		Press KEY2 button LED0 will flash the cycle, and then press KEY0, KEY1 LED0 status will not change
		  
3. UCOSII function used
Here we will not repeat the functions explained before the experiment.
Semaphore creation function 		
OS_EVENT  *OSSemCreate (INT16U cnt)//Create a semaphore with an initial value of cnt
Semaphore send function:
INT8U OSSemPost (OS_EVENT *pevent); / / call once, semaphore counter plus 1
Semaphore request function:
Void OSSemPend (OS_EVENT *pevent, INT16U timeout, INT8U *err) // Request once, semaphore counter minus 1
Semaphore delete function:
? OS_EVENT *OSSemDel (OS_EVENT *pevent, INT8U opt, INT8U *err)//Delete semaphore, semaphore related function will be invalid

4. Experimental description
We don't elaborate on the basic concepts of semaphores. The UCOSII related materials provided by our CD-ROM provide detailed explanations of these concepts. At the same time, it is recommended to read Ren Zhe's book "Principles and Applications of Embedded Real-Time Operating System UCOSII".
The OSTaskCreate() function creates a TaskStart task.
Call the ucos task creation function OSTaskCreate() 5 times in the TaskStart task to create 5 tasks: TaskLed, TaskLed1, TaskLCD, TaskKey, TaskTouch,
The semaphore creation function Sem_Event=OSSemCreate(4) is called to create the semaphore Sem_Event. Then call the OSTaskSuspend() function
The TaskStart() task hangs because after the 5 tasks are created, the tasks done by the TaskStart task have been completed and the task is suspended. Then 5 tasks are executed in a loop;

TaskLed: LED0 is inverted every 600ms if the request is valid.
TaskLed1: LED1 reverses every 600ms
TaskLCD: Color change of a certain area in the upper part of the LCD
TaskKey: Scans the key value every 20ms.
TaskTouch: Scans the touch point of the lower half of the touch screen every 2ms and displays it on the LCD. That is our touchpad program.

In the task TaskKey, we cycle through the key values. If KEY0 is pressed, the semaphore send function will be called to send 10 semaphores, and the semaphore counter will increase by 10.
?For(i=0;i<10;i++)
OSSemPost(Sem_Event); //Continuously send semaphore 10 times
At this time, the task TaskLed is requesting the semaphore wait state, and the task will enter the ready state, and it will be seen that LED0 flashes 5 times.
If KEY1 is pressed, the semaphore send function will be called to send the semaphore once, and the semaphore counter will increase by 1. At this time, the LED0 state reversal can be seen.
??????OSSemPost(Sem_Event); //Send semaphore once
If the KEY2 (WK_UP) button is pressed, the semaphore delete function will be called to delete the semaphore and the semaphore will be invalid. At this time, LED0 resumes flashing.
?OSSemDel(Sem_Event, OS_DEL_ALWAYS, &err);
?The parameter OS_DEL_ALWAYS indicates that the semaphore is deleted immediately.

*/
#include "main.h"
time_t timenow = 1564551507;




OS_EVENT * Sem_Event;
OS_SEM_DATA result[];
u8 err; 

 void Load_Drow_Dialog(void)
{    
	printf("Load_Drow_Dialog\r\n");
}
 
 void Load_Adjust_Panel()
 {
	 printf("Load_Adjust_Panel\r\n");
		//Save_Adjdata();	 
 }




 int main(void)
 {
	 SystemInit();
	 delay_init(72);	     //Delayed initialization
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 GPIO_config();
	 SPI1_Init();
	 uart_init(115200);
	 printf("uC OS II Test, OS_VERSION: %d\r\n",OS_VERSION);
	 LED_Init();
	 KEY_Init();
	 //Thu vien tang cao
	 w5500_lib_init();
	//AT24CXX_Init();		//IIC initialization 
	//Remote_Init();

	delay_ms(500);
	//Load_Drow_Dialog();

	OSInit();
	OSTaskCreate( TaskStart,	//task pointer
					(void *)0,	//parameter
					(OS_STK *)&TASK_START_STK[START_STK_SIZE-1],	//task stack top pointer
					START_TASK_Prio );	//task priority
					

	
	OSStart();
	return 0;

 }	
  
//Start task
void TaskStart(void * pdata)
{
	INT8U err;
	pdata = pdata; 
	printf("TaskStart\r\n");
	OS_ENTER_CRITICAL(); 
	Sem_Event=OSSemCreate(5);  //Create semaphore
	err = OSTaskCreate(TaskLed,	(void * )0,	(OS_STK *)&TASK_LED_STK[LED_STK_SIZE-1], LED_TASK_Prio);
	err = OSTaskCreate(TaskLed1, (void * )0, (OS_STK *)&TASK_LED1_STK[LED1_STK_SIZE-1], LED1_TASK_Prio);
 	err = OSTaskCreate(TaskLCD, (void * )0, (OS_STK *)&TASK_LCD_STK[LCD_STK_SIZE-1], LCD_TASK_Prio);
  err = OSTaskCreate(TaskKey, (void * )0, (OS_STK *)&TASK_KEY_STK[KEY_STK_SIZE-1], KEY_TASK_Prio);
 	err = OSTaskCreate(KeepTime, (void * )0, (OS_STK *)&TASK_KT_STK[KT_STK_SIZE-1], KT_TASK_Prio);
	
	if (err != OS_ERR_NONE) {
		printf("OSTaskCreate() failed: Err = %d\n", 
		       (int)err);
	}
	
 	OSTaskSuspend(START_TASK_Prio);	//suspend but not delete
	OS_EXIT_CRITICAL();
}
//Task 1
// Control the DS0 light off.
void TaskLed(void *pdata)
{
	while(1)
	{	
		OSSemPend(Sem_Event,0,&err); //Request semaphore, if the semaphore is valid (cnt>0), then the task continues to run, otherwise it enters the wait state
		printf("TaskLed\r\n");
		//LED0=!LED0;
		OSTimeDlyHMSM(0,0,1,500);	
	}
}
//Task 2
// Control the lighting of DS1.
void TaskLed1(void *pdata)
{
	while(1)
	{    
		//LED0=!LED0;
		OSTimeDlyHMSM(0,0,1,0);	
		printf("TaskLed1\r\n");
		PHYStatus_Check();
	}
}

//Task 3
// button detection 
 void TaskKey(void *pdata)
 {
	//u8  i=0;
   //printf("TaskKey\r\n");
    while(1){
		if(KEY0==0)
		{
			printf("BT pressed :%d!\r\n",OSTimeGet());
			
			//for(i=0;i<10;i++)
			//	OSSemPost(Sem_Event); // Continuously send semaphores 10 times
			
			OSSemPost(Sem_Event); //Send a semaphore
			
			//OSSemDel(Sem_Event,OS_DEL_ALWAYS,&err);//After deleting the semaphore Sem_Event, the semaphore is invalid, and all tasks will be unrestricted.
			
		}

	  OSTimeDlyHMSM(0,0,0,20);		 
	}
 }
//Task 4
//LCD
 void TaskLCD(void *pdata){
 	u8  count=0;
	 
    while(1)		   							   
	 {
	  
			OSSemQuery(Sem_Event,result);//Query the state of the semaphore Sem_Event
			count=result->OSCnt;		   //Get the count value of the semaphore Sem_Event
			//printf("Sem_Event count: %d, time %s\r\n",count,ctime(&timenow));//Display the count value of the semaphore Sem_Event
			OSTimeDlyHMSM(0,0,5,0);
	  } 		   
 }	
//Task 5
 void KeepTime(void *pdata)
 {  
 	while(1)
	{   
		//printf("KeepTime\r\n");
		timenow++;
		OSTimeDlyHMSM(0,0,1,0);
	};		
 }
 


