#include "usart.h"
#include <ucos_ii.h>
//UART1 receiver timeout
volatile uint16_t u1out = 50;// 50 is 50ms 


//Add the following code to support the printf function without having to select use MicroLIB
#if 1
#pragma import(__use_no_semihosting)             
//Support functions required by the standard library               
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//Define _sys_exit() to avoid using semihosting mode   
_sys_exit(int x) 
{ 
	x = x; 
} 
//Kiem tra dinh nghia fputc
//Redefining low-level library functions to enable direct use of high-level library functions in the C library
//http://www.keil.com/support/man/docs/armlib/armlib_chr1358938931411.htm
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0)
		;//Loop through until the send is complete  
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
u8 USART_RX_BUF[64];     //接收缓冲,最大64个字节.
//接收状态
//bit7，接收完成标志
//bit6，接收到0x0d
//bit5~0，接收到的有效字节数目
u8 USART_RX_STA=0;       //接收状态标记

void uart_init(u32 Baud){

    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	
  
   //USART Initialization settings
   
	USART_InitStructure.USART_BaudRate = Baud;//Generally set to 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
   

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//Open interrupt
   
    USART_Cmd(USART1, ENABLE);               

}

// USART Receiver buffer
uint8_t USART1_index=0,USART1_rx_data_buff[RX_BUFFER_SIZE0];

void USART1_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;
  char USART1_data;
	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
  OS_EXIT_CRITICAL();
	
	
	/* RXNE handler */
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		//USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		USART1_data=(USART1->DR & (uint16_t)0x01FF);
		u1out = 50;// 50ms
		USART1_rx_data_buff[USART1_index++]=USART1_data;

		if(USART1_index==RX_BUFFER_SIZE0) USART1_index=0;
		//printf("Kiem tra uart 1\r\n");	
	}
	OSIntExit();

} 


void printmcuclk(void)
{
	// Check mcu clock
	RCC_ClocksTypeDef mcu_clk;
	RCC_GetClocksFreq(&mcu_clk);
	printf(">Thach anh: \r\nADCCLK:%d\r\nHCLK:%d\r\nPCLK1:%d\r\nPCLK2:%d\r\nSYSCLK:%d",
																mcu_clk.ADCCLK_Frequency,mcu_clk.HCLK_Frequency,
																mcu_clk.PCLK1_Frequency,mcu_clk.PCLK2_Frequency,mcu_clk.SYSCLK_Frequency);
}