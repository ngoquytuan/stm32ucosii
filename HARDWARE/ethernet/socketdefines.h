#ifndef	W5500_def
#define	W5500_def
#include "stm32f10x.h"                  // Device header
//////////////////////////////////////////////////////////////////////
// SOCKET NUMBER DEFINION  ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#define SOCK_UDPS        0
#define SOCK_DNS         1
#define SOCK_SNTP        2
#define SOCK_agent			 3
#define SOCK_trap				 4
#define SOCK_WEBSERVER   5
#define PORT_WEBSERVER  80
#define MAX_HTTPSOCK		 3

#define DATA_BUF_SIZEHTTP   2048
///////////////////////////////////////////////////////////////////////
// PHYStatus check ////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#define MSEC_PHYSTATUS_CHECK 		1000		// msec
#define PHYStatus_check_enable 1
#endif

