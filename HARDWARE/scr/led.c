 
/*-------------------------------------------------*/
/*         	                                       */
/*           		 LED����           	 		   */
/*                                                 */
/*-------------------------------------------------*/

// Ӳ�����ӣ�
// PD2  LED1
// PA8  LED2
#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�
#include "delay.h"

/*-------------------------------------------------*/
/*����������ʼ��LED1��LED2����                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void led_init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;                     
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOC, GPIO_Pin_13); 				//PD2 ����� 
	 
}

/*-------------------------------------------------*/
/*��������LED1����                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void led1_on(void)
{			
	GPIO_ResetBits(GPIOC, GPIO_Pin_13); 			    //PD2 �����
} 

/*-------------------------------------------------*/
/*��������LED1�ر�                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void led1_off(void)
{		
	GPIO_SetBits(GPIOC, GPIO_Pin_13); 				//PD2 �����
}

/*-------------------------------------------------*/
/*��������LED1״̬                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
int led1_state(void)
{		
	if (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)) 
	{
		return 0;
	} else {
		return 1;
	}
}


