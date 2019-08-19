#ifndef _MYAPP2_CFG_H_
#define _MYAPP2_CFG_H_

#include <ucos_ii.h>
#include "os_cpu.h"
#include <stdio.h>
#include <time.h>
#include "w5500init.h"
#include "led.h"
#include "key.h"
#include "dns.h"
#include "sntp.h"
#include "ntpserver.h"
#include "snmp.h"

#define          TASK_STK_SIZE       64
#define          NTPS_STK_SIZE       80
#define          SNMP_STK_SIZE       200
#define          KeepTimeSIZE				 40
#define          CheckLinkSIZE			 48
#define          GetSNTPSIZE	  		 72

#define          TASK_START_ID       0                               
#define          NTPSID			         1
#define          SNMPID              2
#define          KEEPTIMEID          3
#define          CheckLinkID         4
#define          GetTimeID           5
#define          TASK_5_ID           6
  
#define          TASK_START_PRIO    10                               
#define          NTPSIDPRIO      		 5
#define          SNMPPRIO           12
#define          KEEPTIMEPRIO       13
#define          CheckLinkPRIO      14
#define          GetTimePRIO        15
#define          TASK_5_PRIO        16
  

void starttasks(void);
#endif  /* #ifndef _MYAPP_CFG_H_ */
