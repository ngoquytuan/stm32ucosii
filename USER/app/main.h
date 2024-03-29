/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"                  // Device header
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ucos_ii.h>
#include "myapp2.h"
#include "timerHandler.h"
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "spi.h"
#include "w5500init.h"
#include "sntp.h"
//#include "lcd.h"
//#include "spi.h"
//#include "flash.h"
//#include "mmc_sd.h"
//#include "ff.h"
//#include "integer.h"
//#include "diskio.h"
//#include "text.h"
//#include "24cxx.h"
//#include "fontupd.h"
//#include "stdio.h"
//#include "string.h"
//#include "touch.h"
//#include "includes.h"
//#include "remote.h"

/*

#include "GPIO_STM32F10x.h"             // Keil::Device:GPIO
#include <time.h>
#include "ff.h"
#include "diskio.h"
//user include
#include "rccHandler.h"
#include "uartHandler.h"
#include "stm32f10x_it.h"
#include "timerHandler.h"
#include "delay.h"
#include "spiconfig.h"
#include "w5500init.h"
#include "stm32f1fatfs.h"
#include "socketdefines.h"
#include "ntpserver.h"
#include "snmp.h"
#include "eeprom_stm.h"
#include "httpServer.h"
#include "webpage.h"
#include "sntp.h"
#include "dns.h"
*/

/*
#include "delay.h"
#include "lcd16x2.h"
#include "uart.h"
#include "c8t6f2fucn.h"
#include "crc16.h"
#include "tim.h"
#include "ads1015.h"

#include  "w5500.h"
#include  "string.h"
#include "eeprom_stm.h"
*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
//#define ONTIME 1
//#define STOP   0
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
//void networkSevices();
//void loadwebpages();
//void usart1Process();
void GPIO_config(void);
#endif /* __MAIN_H */

