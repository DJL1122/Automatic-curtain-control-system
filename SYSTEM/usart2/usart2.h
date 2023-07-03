#ifndef __USART2_H
#define	__USART2_H

#include "sys.h" 
#include <stdio.h>

#define USART2_REC_LEN  			24  	//定义最大接收字节数 200
#define USART2_MAX_SEND_LEN		200	  //定义最大发送字节数 200

extern u16 USART2_RX_STA;         		//接收状态标记
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 			//发送缓冲,最大USART3_MAX_SEND_LEN字节
void usart2_init(u32 bound);
void u2_printf(char* fmt,...);
void TIM2_Config(u16 arr,u16 psc);
void TIM2_Set(u8 sta);
#endif /* __USART1_H */

