#ifndef __USART2_H
#define	__USART2_H

#include "sys.h" 
#include <stdio.h>

#define USART2_REC_LEN  			24  	//�����������ֽ��� 200
#define USART2_MAX_SEND_LEN		200	  //����������ֽ��� 200

extern u16 USART2_RX_STA;         		//����״̬���
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 			//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
void usart2_init(u32 bound);
void u2_printf(char* fmt,...);
void TIM2_Config(u16 arr,u16 psc);
void TIM2_Set(u8 sta);
#endif /* __USART1_H */

