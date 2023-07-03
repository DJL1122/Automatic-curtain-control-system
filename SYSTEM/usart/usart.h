#ifndef __USART_H
#define __USART_H

#include "sys.h" 
#include "stdio.h"

#define USART_MAX_SEND_LEN		200	  //定义最大发送字节数 200
#define USART_REC_LEN  			20  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define USART3_RX_EN 			1					//0,不接收;1,接收.
extern u8 uart_st ;

extern u8  USART_TX_BUF[USART_MAX_SEND_LEN]; 			//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void USART1_IRQHandler(void);
void TIM4_Set(u8 sta);
void TIM4_Init(u16 arr,u16 psc);
#endif


