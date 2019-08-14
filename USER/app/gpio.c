#include "stm32f10x.h"                  // Device header
/**
* @brief  GPIO_config cho kit C8T6 china : LED PA1
  * @param  GPIO_PinWrite chu y  GPIO_Pin_0 # 0???
  * @retval  
  */
void GPIO_config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;			//Interrupt parameter interrupt priority

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// Initialize the network module W5500-CS pin (PA0)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	//LED PA1 : cho kit C8T6 china : LED PA1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	// W5500_RST Pin initialization configuration (PA2) 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
	// W5500_INT Pin initialization configuration(PA3)
	//GPIO_PinConfigure(GPIOA, 3, GPIO_IN_PULL_UP, GPIO_MODE_INPUT);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	// Initialize the network module SDCARD-CS pin (PA4)
	//GPIO_PinConfigure(GPIOA, 4, GPIO_OUT_PUSH_PULL, GPIO_MODE_OUT50MHZ);
	//GPIO_PinWrite(GPIOA, 4, 1);
	
	// PC13 : cho kit C8T6 china : CS FLASH
	//GPIO_PortClock   (GPIOC, true);
	//GPIO_PinConfigure(GPIOC, GPIO_Pin_13, GPIO_OUT_PUSH_PULL, GPIO_MODE_OUT50MHZ);
	//GPIO_PinWrite(GPIOC, GPIO_Pin_13, 1);
	
	// Enable the EXTI3 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;				//W5500_INT External interrupt channel
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//Preemption priority 2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//Subpriority 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//Enable external interrupt channel
	NVIC_Init(&NVIC_InitStructure);


  	// Connect EXTI Line3 to PA3 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);


	// PA3 as W5500 interrupt input 
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Check if the system has resumed from IWDG reset */
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  { 
    /* WWDGRST flag set */
    /* Turn on LED1 */
		GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);
    /* Clear reset flags */
    RCC_ClearFlag();
  }
  else
  {
    /* WWDGRST flag is not set */
    /* Turn off LED1 */
    GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_SET);
  }
}

