#ifndef __USART_H
#define __USART_H
#include <stdio.h>
#include "stm32f10x.h"                  // Device header
#define RX_BUFFER_SIZE0  25
extern u8 USART_RX_BUF[64];     
extern u8 USART_RX_STA;         
extern uint8_t USART1_index,USART1_rx_data_buff[];
void uart_init(u32 Baud);

#endif
