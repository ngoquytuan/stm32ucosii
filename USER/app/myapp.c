#include "myapp.h"
extern time_t timenow;
uint8_t TimeIsSet = 0;
//Task stack
OS_STK  TASK_LED1_STK[LED_STK_SIZE];
OS_STK  TASK_LED_STK[LED1_STK_SIZE];
OS_STK  TASK_LCD_STK[LCD_STK_SIZE];
OS_STK  TASK_START_STK[START_STK_SIZE];
OS_STK  TASK_KEY_STK[KEY_STK_SIZE];
OS_STK  TASK_KT_STK[KT_STK_SIZE];
OS_STK  TASK_CheckETH_STK[MIN_STK_SIZE];
OS_STK  TASKGetTimeFromNTPServerSTK[MIN_STK_SIZE];
/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/
OS_EVENT * Sem_Event;
OS_SEM_DATA result[];
 
//Memory management related variables
OS_MEM * CommTxBuffer1;
INT8U CommTxPart1[8][128];

//Message mailbox related variable
OS_EVENT * Str_box;

//Semaphore related variable
OS_EVENT * Str_semp;

//Mutually exclusive semaphore
OS_EVENT * Str_mutex;

//Timer related variable
OS_TMR * Str_Tmr;

void  TmrCallBack(OS_TMR *ptmr, void *p_arg);
void  TmrCallBack(OS_TMR *ptmr, void *p_arg)
{
	printf("Timed task\r\n");  //Prompt message
}

void starttasks(void)
{
	INT8U  err;
	
	OS_STK *ptos;
  OS_STK *pbos;
  INT32U  size;
	
	ItDisAll();/* For an embedded target, disable all interrupts until we are ready to accept them */
	OSInit();/* Initialize "uC/OS-II, The Real-Time Kernel"                                      */
	
	

	ptos        = &TASK_START_STK[START_STK_SIZE - 1];
	pbos        = &TASK_START_STK[0];
	size        = START_STK_SIZE;
	//OSTaskStkInit(TaskStart, &pbos, &size,OS_TASK_OPT_NONE); 
	err = OSTaskCreateExt(TaskStart,
												(void *)0,
												ptos,
												START_TASK_Prio,
												START_TASK_Prio,//ID
												pbos,
												size,
												(void *)0,
												OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
												
	/*err = OSTaskCreate( TaskStart,	//task pointer
					(void *)0,	//parameter
					(OS_STK *)&TASK_START_STK[START_STK_SIZE-1],	//task stack top pointer
					START_TASK_Prio );	//task priority
					*/
	if (err != OS_ERR_NONE) {
		printf("OSTaskCreate() failed: Err = %d\n", 
		       (int)err);
	}					
	//OSTaskNameSet(START_TASK_Prio, (INT8U *)(void *)"Start Task"   , &err);
	OSStart();
}

/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
* Arguments   : p_arg   is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*               2) Interrupts are enabled once the task start because the I-bit of the CCR register was
*                  set to 0 by 'OSTaskCreate()'.
*********************************************************************************************************
*/
void TaskStart(void * pdata)
{
	#if OS_CRITICAL_METHOD == 3u                         /* Allocate storage for CPU status register       */
    OS_CPU_SR  cpu_sr = 0u;
	#endif
	INT8U err;
	
	pdata = pdata; 
	printf("TaskStart\r\n");
	#if OS_TASK_STAT_EN > 0
	OSStatInit();                                /* Determine CPU capacity                                                     */
	#endif
	
	Str_Tmr       = OSTmrCreate(0,
		                        1000,
								OS_TMR_OPT_PERIODIC,
								(OS_TMR_CALLBACK)TmrCallBack,
								(void *)0,
								(INT8U *)(void *)"Tmr test",
								&err);                      //Create a soft timer
	OSTmrStart(Str_Tmr,&err);                    //Start soft timer							
	//OS_ENTER_CRITICAL(); 
	//Sem_Event=OSSemCreate(5);  //Create semaphore
	err = OSTaskCreateExt(TaskLed,	(void * )0,	(OS_STK *)&TASK_LED_STK[LED_STK_SIZE-1], LED_TASK_Prio, LED_TASK_Prio,&TASK_LED_STK[0],LED_STK_SIZE,(void *)0,OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	err = OSTaskCreate(TaskLed1, (void * )0, (OS_STK *)&TASK_LED1_STK[LED1_STK_SIZE-1], LED1_TASK_Prio);
 	err = OSTaskCreate(TaskLCD, (void * )0, (OS_STK *)&TASK_LCD_STK[LCD_STK_SIZE-1], LCD_TASK_Prio);
  err = OSTaskCreate(TaskKey, (void * )0, (OS_STK *)&TASK_KEY_STK[KEY_STK_SIZE-1], KEY_TASK_Prio);
 	err = OSTaskCreate(KeepTime, (void * )0, (OS_STK *)&TASK_KT_STK[KT_STK_SIZE-1], KT_TASK_Prio);
	err = OSTaskCreate(TaskCheckLinkEth, (void * )0, (OS_STK *)&TASK_CheckETH_STK[MIN_STK_SIZE-1], TaskCheckLinkEthPrio);
	err = OSTaskCreate(TaskGetTimeFromNTPServer, (void * )0, (OS_STK *)&TASKGetTimeFromNTPServerSTK[MIN_STK_SIZE-1], TaskGetTimeFromNTPServerPrio);
	if (err != OS_ERR_NONE) {
		printf("OSTaskCreate() failed: Err = %d\n", 
		       (int)err);
	}
	
 	OSTaskSuspend(START_TASK_Prio);	//suspend but not delete
	//OS_EXIT_CRITICAL();
}

//Task 1
// Control the DS0 light off.
void TaskLed(void *pdata)
{
	INT8U err;
	while(1)
	{	
		//OSSemPend(Sem_Event,0,&err); //Request semaphore, if the semaphore is valid (cnt>0), then the task continues to run, otherwise it enters the wait state
		printf("TaskLed\r\n");
		//LED0=!LED0;
		OSTimeDlyHMSM(0,0,1,500);	
	}
}
//Task 2
// Control the lighting of DS1.
void TaskLed1(void *pdata)
{
	//INT8U  err;
	pdata = pdata;
	
	while(1)
	{    
		//LED0=!LED0;
		OSTimeDlyHMSM(0,0,1,0);	
		//printf("TaskLed1\r\n");
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
		DNS_time_handler();
		OSTimeDlyHMSM(0,0,1,0);
	};		
 }
 

void TaskCheckLinkEth(void *pdata)
{
	while(1)
	{    
		OSTimeDlyHMSM(0,0,5,0);	
		PHYStatus_Check();
	}
}

void TaskGetTimeFromNTPServer(void *pdata)
{
	while(1)
	{    
		//printf("TaskGetTimeFromNTPServer\r\n");
		if(TimeIsSet == 1) TimeIsSet = 0;		
		if(SNTP_run() == 1) TimeIsSet = 1;
		
		if(TimeIsSet == 0) 
					OSTimeDlyHMSM(0,0,0,500);
		else 	OSTimeDlyHMSM(0,2,0,0);	
	}
}
 

 