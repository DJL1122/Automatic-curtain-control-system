#ifndef __USART_H
#define __USART_H

#include "sys.h" 
#include "stdio.h"

#define USART_MAX_SEND_LEN		200	  //����������ֽ��� 200
#define USART_REC_LEN  			20  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define USART3_RX_EN 			1					//0,������;1,����.
extern u8 uart_st ;

extern u8  USART_TX_BUF[USART_MAX_SEND_LEN]; 			//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void USART1_IRQHandler(void);
void TIM4_Set(u8 sta);
void TIM4_Init(u16 arr,u16 psc);
#endif


