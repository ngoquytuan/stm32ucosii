#include "myapp2.h"
#include "usart.h"
extern time_t timenow;
uint8_t TimeIsSet = 0;
extern volatile uint16_t u1out;
OS_STK_DATA result[];
OS_STK        TaskStartStk[TASK_STK_SIZE];                           
OS_STK        NTPServerStk[NTPS_STK_SIZE];
OS_STK        SNMPStk[SNMP_STK_SIZE];           
OS_STK        KeepTimeStk[KeepTimeSIZE];
OS_STK        CheckLinkStk[CheckLinkSIZE];
OS_STK        GetSNTPStk[GetSNTPSIZE];
void  TaskStart (void *pdata)
{
	//OS_CPU_SR  cpu_sr;
	pdata = pdata;
  
	while(1)
	{	
		//OS_ENTER_CRITICAL();
		
		//OS_EXIT_CRITICAL();
		OSTimeDlyHMSM(0,0,5,0);
		OSTaskStkChk(TASK_START_PRIO,result);
		printf("TaskStart OSFree :%d, OSUsed :%d \r\n",result->OSFree,result->OSUsed);
		OSTaskStkChk(NTPSIDPRIO,result);
		printf("NTPServer OSFree :%d, OSUsed :%d \r\n",result->OSFree,result->OSUsed);
		OSTaskStkChk(SNMPPRIO,result);
		printf("SNMP      OSFree :%d, OSUsed :%d \r\n",result->OSFree,result->OSUsed);
		OSTaskStkChk(KEEPTIMEPRIO,result);
		printf("KEEPTIME  OSFree :%d, OSUsed :%d \r\n",result->OSFree,result->OSUsed);
		OSTaskStkChk(CheckLinkPRIO,result);
		printf("CheckLink OSFree :%d, OSUsed :%d \r\n",result->OSFree,result->OSUsed);
		OSTaskStkChk(GetTimePRIO,result);
		printf("GetTime   OSFree :%d, OSUsed :%d \r\n",result->OSFree,result->OSUsed);
		if(u1out == 50) 
		{	u1out = 0;
			printf("UART1:%s\r\n",USART1_rx_data_buff);
			for(USART1_index=0;USART1_index<RX_BUFFER_SIZE0;USART1_index++)
															{
															USART1_rx_data_buff[USART1_index]=0;
															}  
															USART1_index=0;
		}
		//OSIntExit();  
	}
}
//Task NTP server
void  NTPServer (void *pdata)
{
	int32_t ret = 0;
	pdata = pdata;
  printf("NTPServer started!\r\n");
	TIM3->CNT = 0;
	ntpserverdefaultconfig();
	while(1)
	{	
		//printf("NTPServer is running!\r\n");
		//printf("TIM :%d\r\n",TIM3->CNT);
		//TIM3->CNT = 0;
		if( (ret = NTPUDP(SOCK_UDPS)) < 0) {
			printf("SOCKET ERROR : %d\r\n", ret);
		}
		OSTimeDlyHMSM(0,0,2,0);
	}
}

//Task SNMP(Simple Network Management Protocol) Agent Initialize
void SNMP(void *pdata)
{
	pdata = pdata;
	printf("SNMP started!\r\n");
	// NMS (SNMP manager) IP address
	snmpd_init(managerIP,agentIP,SOCK_agent,SOCK_trap);	
	while(1)
	{
		//printf("SNMP started!\r\n");
			//SNMPv1 run
			//Run SNMP Agent Fucntion
			/* SNMP Agent Handler */
			//SMI Network Management Private Enterprise Codes: : moi cong ty phai dang ky 1 so rieng, 
			//tham khao : https://www.iana.org/assignments/enterprise-numbers/enterprise-numbers
			// Vi du Arduino : 36582
    	// SNMP Agent daemon process : User can add the OID and OID mapped functions to snmpData[] array in snmprun.c/.h
			// [net-snmp version 5.7 package for windows] is used for this demo.
			// * Command example
    	// [Command] Get:			  snmpget -v 1 -c public 192.168.1.246 .1.3.6.1.2.1.1.1.0 			// (sysDescr)
    	// [Command] Get: 			snmpget -v 1 -c public 192.168.1.246 .1.3.6.1.4.1.6.1.0 			// (Custom, get LED status)
    	// [Command] Get-Next: 	snmpwalk -v 1 -c public 192.168.1.246 .1.3.6.1
			// [Command] Set: 			snmpset -v 1 -c public 192.168.1.246 .1.3.6.1.4.1.6.1.1 i 1			// (Custom, LED 'On')
    	// [Command] Set: 			snmpset -v 1 -c public 192.168.1.246 .1.3.6.1.4.1.6.1.1 i 0			// (Custom, LED 'Off')
			snmpd_run();	
	
		OSTimeDlyHMSM(0,0,0,50);
	}
}

void KeepTime(void *pdata)
{  
	pdata = pdata;
 	while(1)
	{   
		//printf("KeepTime\r\n");
		timenow++;
		DNS_time_handler();
		OSTimeDlyHMSM(0,0,1,0);
	};		
}

void CheckLinkEth(void *pdata)
{
	pdata = pdata;
	while(1)
	{    
		OSTimeDlyHMSM(0,0,5,0);	
		PHYStatus_Check();
	}
}

void GetTimeFromNTPServer(void *pdata)
{
	pdata = pdata;
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

void starttasks(void)
{
	INT8U  err;
	
	OS_STK *ptos;
  OS_STK *pbos;
  INT32U  size;
	printf("My apps start\r\n");
	ItDisAll();/* For an embedded target, disable all interrupts until we are ready to accept them */
	OSInit();/* Initialize "uC/OS-II, The Real-Time Kernel"                                      */
	
	

	ptos        = &TaskStartStk[TASK_STK_SIZE - 1];  
	pbos        = &TaskStartStk[0];
	size        = TASK_STK_SIZE;
	//OSTaskStkInit(TaskStart, &pbos, &size,OS_TASK_OPT_NONE); 
	
	err = OSTaskCreateExt(TaskStart,
												(void *)0,
												ptos,
												TASK_START_PRIO,
												TASK_START_ID,//ID
												pbos,
												size,
												(void *)0,
												OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
												
	err = OSTaskCreateExt(NTPServer,
												(void *)0,
												&NTPServerStk[NTPS_STK_SIZE - 1],
												NTPSIDPRIO,
												NTPSID,//ID
												&NTPServerStk[0],
												NTPS_STK_SIZE,
												(void *)0,
												OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	err = OSTaskCreateExt(SNMP,
												(void *)0,
												&SNMPStk[SNMP_STK_SIZE - 1],
												SNMPPRIO,
												SNMPID,//ID
												&SNMPStk[0],
												SNMP_STK_SIZE,
												(void *)0,
												OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
  err = OSTaskCreateExt(KeepTime,
												(void *)0,
												&KeepTimeStk[KeepTimeSIZE - 1],
												KEEPTIMEPRIO,
												KEEPTIMEID,//ID
												&KeepTimeStk[0],
												KeepTimeSIZE,
												(void *)0,
												OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);		

  err = OSTaskCreateExt(CheckLinkEth,
												(void *)0,
												&CheckLinkStk[CheckLinkSIZE - 1],
												CheckLinkPRIO,
												CheckLinkID,//ID
												&CheckLinkStk[0],
												CheckLinkSIZE,
												(void *)0,
												OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	err = OSTaskCreateExt(GetTimeFromNTPServer,
												(void *)0,
												&GetSNTPStk[GetSNTPSIZE - 1],
												GetTimePRIO,
												GetTimeID,//ID
												&GetSNTPStk[0],
												GetSNTPSIZE,
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




