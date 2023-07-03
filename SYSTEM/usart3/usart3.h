#ifndef __USART3_H
#define	__USART3_H

#include "sys.h" 
#include <stdio.h>

#define USART3_REC_LEN  			200  	//定义最大接收字节数 200
#define USART3_MAX_SEND_LEN		200	  //定义最大发送字节数 200

extern u16 USART3_RX_STA;         		//接收状态标记
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//发送缓冲,最大USART3_MAX_SEND_LEN字节
void usart3_init(u32 bound);
void u3_printf(char* fmt,...);

#endif /* __USART1_H */

