
#include "main.h"
time_t timenow = 1564551507;


int main(void)
 {
	 SystemInit();
	 delay_init(72);	     //Delayed initialization
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 GPIO_config();
	 SPI1_Init();
	 Timer_Configuration();
	 uart_init(115200);
	 printf("uC OS II Test, OS_VERSION: %d, if not run, check link cable\r\n",OS_VERSION);
	 LED_Init();
	 KEY_Init();
	 //Thu vien tang cao
	 w5500_lib_init();
	 //Get time from ntp time server
	 SNTP_init();
	 loadwebpages();
	 delay_ms(500);
	 starttasks();
	 return 0;

 }	
  

